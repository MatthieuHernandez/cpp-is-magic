#include <cassert>
#include <cmath>
#include <compare>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numbers>

template <typename CT>
class more_precise {
    using T = typename std::remove_const<CT>::type;

  public:
    more_precise(const T& v = {}) {
        value_1 = v;
        split_values();
    }

    T get() const { return value_1 + value_2; }

    more_precise(const more_precise<T>&& other) { this->operator=(other); }

    more_precise(const more_precise<T>& other) { this->operator=(other); }

    more_precise<T>& operator=(const more_precise<T>&& other) {
        value_1 = other.value_1;
        value_2 = other.value_2;
        split_values();
        return *this;
    }

    more_precise<T>& operator=(const more_precise<T>& other) {
        value_1 = other.value_1;
        value_2 = other.value_2;
        split_values();
        return *this;
    }

    more_precise<T> operator+(const more_precise<T>& other) const {
        more_precise<T> result(*this);
        result.value_1 += other.value_1;
        result.value_2 += other.value_2;
        return result;
    }

    more_precise<T> operator-(const more_precise<T>& other) {
        more_precise<T> result(*this);
        result.value_1 -= other.value_1;
        result.value_2 -= other.value_2;
        return result;
    }

    more_precise<T> operator*(more_precise<T> other) {
        more_precise<T> result(*this);
        result.value_1 = result.value_1 * other.value_1 + result.value_1 * other.value_2;
        result.value_2 = result.value_2 * other.value_1 + result.value_2 * other.value_2;
        return result;
    }

    more_precise<T> operator/(more_precise<T> other) {
        more_precise<T> result(*this);
        result.value_1 = result.value_1 / (other.value_1 + other.value_2);
        result.value_2 = result.value_2 / (other.value_1 + other.value_2);
        return result;
    }

    std::partial_ordering operator<=>(const more_precise<T>& other) const {
        if (value_1 < other.value_1) {
            return std::partial_ordering::less;
        } else if (value_1 > other.value_1) {
            return std::partial_ordering::greater;
        } else if (value_1 == other.value_1 && value_2 < other.value_2) {
            return std::partial_ordering::less;
        } else if (value_1 == other.value_1 && value_2 > other.value_2) {
            return std::partial_ordering::greater;
        } else if (value_1 == other.value_1 && value_1 == other.value_2) {
            return std::partial_ordering::equivalent;
        } else {
            return std::partial_ordering::unordered;
        }
    }

    void split_values() {
        counter++;
        static constexpr T split_factor = 1024;
        if (value_2 < value_1 / split_factor) {
            T tmp_1 = value_1 * split_factor;
            tmp_1 = std::floor(tmp_1);
            tmp_1 = tmp_1 / split_factor;
            value_2 += (value_1 - tmp_1);
            value_1 = tmp_1;
        }
    }
    static int32_t counter;
    T value_1{};
    T value_2{};
};

template <>
int32_t more_precise<float>::counter = 0;

template <>
int32_t more_precise<double>::counter = 0;

template <>
int32_t more_precise<long double>::counter = 0;

template <typename T>
T get_epsilon() {
    if (typeid(T) == typeid(float) || typeid(T) == typeid(more_precise<float>)) {
        return 1e-7;
    } else if (typeid(T) == typeid(double) || typeid(T) == typeid(more_precise<double>)) {
        return 1e-14;
    } else if (typeid(T) == typeid(long double) || typeid(T) == typeid(more_precise<long double>)) {
        return 1e-14;
    }
    assert(false);
    return 0;
}

template <typename T>
T square_root(T n) {
    T zero = 0.0;
    T epsilon = get_epsilon<T>();
    T guess = n;
    T previous_guess;
    T diff;
    T two = 2.0;
    do {
        previous_guess = guess;
        guess = (guess + n / guess) / two;
        diff = guess - previous_guess;
    } while (diff > epsilon || diff < (zero - epsilon));

    return guess;
}

template <typename T>
T gauss_legendre_algorithm() {
    static uint8_t n = 5;
    static T a = 1.0l;
    static T b = 1.0l / std::sqrt(2.0l);
    static T p = 1.0l;
    static T two = 2.0l;
    static T four = 4.0l;
    static T t = 1.0l / 4.0l;
    if (n == 0) {
        return ((a + b) * (a + b) / (four * t));
    }
    T a_next = (a + b) / two;
    b = square_root(a * b);
    t = t - p * (a - a_next) * (a - a_next);
    p = two * p;
    a = a_next;
    n--;
    return gauss_legendre_algorithm<T>();
}

int main() {
    auto compare_pi = [](auto t) {
        using T = decltype(t);
        constexpr T std_pi = std::numbers::pi_v<T>;
        T my_pi = gauss_legendre_algorithm<T>();
        T my_more_precise_pi = gauss_legendre_algorithm<more_precise<T>>().get();
        std::cout << std::setprecision(23) << std_pi << std::endl;
        std::cout << std::setprecision(23) << my_pi << std::endl;
        std::cout << std::setprecision(23) << my_more_precise_pi << std::endl << std::endl;
        std::cout << "counter = " << more_precise<T>::counter << std::endl << std::endl;
        // assert(my_pi != std_pi);
        // assert(my_more_precise_pi == std_pi);
        return 0;
    };

    auto iterate_types = [&](auto... types) { (compare_pi(types), ...); };

    using long_double = long double;
    iterate_types(float{}, double{}, long_double{});
    return 0;
}