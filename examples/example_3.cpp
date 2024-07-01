#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <string_view>

long double square_root(long double n) {
    long double epsilon = 1e-12;
    long double guess = n;
    long double previous_guess;
    do {
        previous_guess = guess;
        guess = (guess + n / guess) / 2.0l;
    } while (std::fabs(guess - previous_guess) > epsilon);

    return guess;
}

template <typename T>
class more_precise {
  public:
    more_precise(T v) {
        // T x = 4294967296;
        // v *= x;
        value = frexp(v, &power);
        // value /= x;
    }

    more_precise(T v, int p) : value(v), power(p) {}

    more_precise<T> operator+(more_precise<T> other) {
        if (power == other.power) return more_precise<T>(value + other.value, power);
        return more_precise<T>(ldexp(value, power) + ldexp(other.value, other.power));
    }

    more_precise<T> operator-(more_precise<T> other) {
        if (power == other.power) return more_precise<T>(value - other.value, power);
        return more_precise<T>(ldexp(value, power) - ldexp(other.value, other.power));
    }

    more_precise<T> operator*(more_precise<T> other) {
        T v = value * other.value;
        int p = power + other.power;
        return more_precise<T>(v, p);
    }

    more_precise<T> operator/(more_precise<T> other) {
        T v = value / other.value;
        int p = power - other.power;
        return more_precise<T>(v, p);
    }

    more_precise<T> sqrt() {
        T v;
        if (power % 2 == 0) {
            v = square_root(value);
            int p = power / 2;
            return more_precise<T>(v, p);
        } else {
            v = square_root(ldexp(value, power));
            return more_precise<T>(v);
        }
    }

    T get() { return ldexp(value, power); }

  private:
    T value;
    int power;
};

template <typename T>
auto to_bitset(T value) {
    const size_t s = sizeof(T);
    const size_t bits = sizeof(T) * 8;
    unsigned char bytes[s];
    std::memcpy(bytes, &value, s);
    std::bitset<bits> bitset;
    for (size_t i = 0; i < s; ++i) {
        for (size_t bit = 0; bit < 8; ++bit) {
            bitset[i * 8 + bit] = (bytes[i] >> bit) & 1;
        }
    }
    return bitset;
};

template <typename T>
T pi_brent_salamin() {
    static uint8_t n = 5;
    static T a = 1.0l;
    static T b = 1.0l / std::sqrt(2.0l);
    static T p = 1.0l;
    static T t = 1.0l / 4.0l;
    if (n == 0) {
        return ((a + b) * (a + b) / (4.0l * t));
    }
    T a_next = (a + b) / 2.0l;
    b = std::sqrt(a * b);
    t = t - p * (a - a_next) * (a - a_next);
    p = 2.0l * p;
    a = a_next;
    n--;
    return pi_brent_salamin<T>();
}

template <typename T>
T pi_brent_salamin_2() {
    static uint8_t n = 5;
    static more_precise<T> a = more_precise<T>(1.0l);
    static more_precise<T> b = more_precise<T>(1.0l) / more_precise<T>(2.0l).sqrt();
    static more_precise<T> p = more_precise<T>(1.0l);
    static more_precise<T> t = more_precise<T>(0.25l);
    if (n == 0) {
        return ((a + b) * (a + b) / (more_precise<T>(4.0l) * t)).get();
    }
    more_precise<T> a_next = (a + b) / more_precise<T>(2.0l);
    auto tmp = (a * b);
    b = tmp.sqrt();
    t = t - p * (a - a_next) * (a - a_next);
    p = more_precise<T>(2.0l) * p;
    a = a_next;
    n--;
    return pi_brent_salamin_2<T>();
}

int main() {
    auto my_pi = pi_brent_salamin<double>();
    std::cout << std::setprecision(20) << my_pi << std::endl;
    std::cout << "0b" << to_bitset(my_pi) << std::endl;

    auto my_pi_2 = pi_brent_salamin_2<double>();
    std::cout << std::setprecision(20) << my_pi_2 << std::endl;
    std::cout << "0b" << to_bitset(my_pi_2) << std::endl;

    auto std_pi = std::numbers::pi_v<double>;
    std::cout << std::setprecision(20) << std_pi << std::endl;
    std::cout << "0b" << to_bitset(std_pi) << std::endl;

    // assert(pi_brent_salamin<float>() == std::numbers::pi_v<float>);
    // assert(pi_brent_salamin<double>() == std::numbers::pi_v<double>);

    return 0;
}