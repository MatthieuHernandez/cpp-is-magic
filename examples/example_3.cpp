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
    constexpr more_precise(const T& v = {}) {
        value_1 = v;
        split_values();
    }

    constexpr T get() const { return value_1 + value_2; }

    constexpr more_precise(const more_precise<T>&& other) { this->operator=(other); }

    constexpr more_precise(const more_precise<T>& other) { this->operator=(other); }

    constexpr more_precise<T>& operator=(const more_precise<T>&& other) {
        value_1 = other.value_1;
        value_2 = other.value_2;
        split_values();
        return *this;
    }

    constexpr more_precise<T>& operator=(const more_precise<T>& other) {
        value_1 = other.value_1;
        value_2 = other.value_2;
        split_values();
        return *this;
    }

    constexpr more_precise<T> operator+(const more_precise<T>& other) const {
        more_precise<T> result;
        result.value_1 = value_1 + other.value_1;
        result.value_2 = value_2 + other.value_2;
        return result;
    }

    constexpr more_precise<T> operator-(const more_precise<T>& other) const {
        more_precise<T> result;
        result.value_1 = value_1 - other.value_1;
        result.value_2 = value_2 - other.value_2;
        return result;
    }

    constexpr more_precise<T> operator*(const more_precise<T>& other) const {
        more_precise<T> result;
        result.value_1 = result.value_1 * other.value_1 + result.value_1 * other.value_2;
        result.value_2 = result.value_2 * other.value_1 + result.value_2 * other.value_2;
        return result;
    }

    constexpr more_precise<T> operator/(const more_precise<T>& other) const {
        more_precise<T> result;
        result.value_1 = result.value_1 / (other.value_1 + other.value_2);
        result.value_2 = result.value_2 / (other.value_1 + other.value_2);
        return result;
    }

    constexpr bool operator==(const more_precise<T>& other) const = default;

    constexpr std::partial_ordering operator<=>(const more_precise<T>& other) const {
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

    constexpr void split_values() {
        static constexpr T split_factor = 256 * sizeof(T);
        if ((value_2 < value_1 && value_1 > 0) || (value_2 > value_1 && value_1 < 0)) {
            T tmp_1 = value_1 * split_factor;
            // Equivalent to std::floor which is not yet constexpr.
            tmp_1 = static_cast<T>(static_cast<long long>(tmp_1));
            tmp_1 = tmp_1 / split_factor;
            value_2 += (value_1 - tmp_1);
            value_1 = tmp_1;
        }
    }
    T value_1{};
    T value_2{};
};

template <typename T>
static constexpr T square_root(const T& x) {
    constexpr T zero = 0.0l;
    if (x == zero) {
        return x;
    }
    T guess = x;
    T previous_guess;
    constexpr T two = 2.0;
    for (int8_t i = 50; i > 0; i--) {
        previous_guess = guess;
        guess = (guess + x / guess) / two;
    }
    return guess;
}

template <typename T>
static constexpr T gauss_legendre_algorithm() {
    T two = 2.0l;
    T four = 4.0l;
    T a = 1.0l;
    T b = 1.0l / square_root(2.0l);
    T p = 1.0l;
    T t = 1.0l / 4.0l;
    for (int8_t i = 5; i > 0; i--) {
        T a_next = (a + b) / two;
        b = square_root(a * b);
        t = t - p * (a - a_next) * (a - a_next);
        p = two * p;
        a = a_next;
    }
    return ((a + b) * (a + b) / (four * t));
}

int main() {
    // float my_more_precise_pi = gauss_legendre_algorithm<more_precise<float>>().get();

    auto compare_pi = [](auto t) {
        using T = decltype(t);
        constexpr T std_pi = std::numbers::pi_v<T>;
        constexpr T my_pi = gauss_legendre_algorithm<T>();
        T my_more_precise_pi = gauss_legendre_algorithm<more_precise<T>>().get();
        std::cout << std::setprecision(23) << std_pi << std::endl;
        std::cout << std::setprecision(23) << my_pi << std::endl;
        std::cout << std::setprecision(23) << my_more_precise_pi << std::endl << std::endl;
        // assert(my_pi != std_pi);
        // assert(my_more_precise_pi == std_pi);
        return 0;
    };

    auto iterate_types = [&](auto... types) { (compare_pi(types), ...); };

    using long_double = long double;
    iterate_types(long_double{});
    return 0;
}