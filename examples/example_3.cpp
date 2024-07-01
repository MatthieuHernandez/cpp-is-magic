#include <cassert>
#include <cmath>
#include <cstdint>
#include <numbers>

template <typename T>
T pi_brent_salamin() {
    static uint8_t n = 5;
    static auto a = 1.0l;
    static auto b = 1.0l / std::sqrt(2.0l);
    static auto p = 1.0l;
    static auto t = 1.0l / 4.0l;
    if (n == 0) {
        return ((a + b) * (a + b) / (4.0l * t));
    }
    auto a_next = (a + b) / 2.0l;
    b = std::sqrt(a * b);
    t = t - p * (a - a_next) * (a - a_next);
    p = 2.0l * p;
    a = a_next;
    n--;
    return pi_brent_salamin<T>();
}

int main() {
    assert(pi_brent_salamin<float>() == std::numbers::pi_v<float>);
    assert(pi_brent_salamin<double>() == std::numbers::pi_v<double>);
    return 0;
}