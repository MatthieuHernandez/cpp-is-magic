#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <string_view>

double pi_Leibniz_formula(uint64_t n) {
    double sum_1 = 0.0;
    double sum_2 = 0.0;
    int sign = 1;
    for (uint64_t i = 0; i < n; ++i) {
        sum_1 += sign / (2.0 * i + 1.0);
        sign *= -1;
        if (n == 1000) {
            sum_2 = sum_1;
            sum_1 = 0.0;
        }
    }
    return 4.0 * (sum_1 + sum_2);
}

double pi_brent_salamin(uint8_t n) {
    static double a = 1.0;
    static double b = 1.0 / std::sqrt(2.0);
    static double p = 1.0;
    static double t = 1.0 / 4.0;
    if (n == 0) {
        return (a + b) * (a + b) / (4.0 * t);
    }
    double a_next = (a + b) / 2.0;
    double b_next = std::sqrt(a * b);
    double t_next = t - p * (a - a_next) * (a - a_next);
    double p_next = 2.0 * p;

    a = a_next;
    b = b_next;
    t = t_next;
    p = p_next;

    return pi_brent_salamin(n - 1);
}

uint64_t to_int(double& d) {
    uint64_t u;
    memcpy(&u, &d, sizeof(d));
    return u;
}

int main() {
    std::cout << "Calculation..." << std::endl;
    auto mi_pi = pi_brent_salamin(2);
    std::cout << std::setprecision(16) << mi_pi << std::endl;
    std::cout << "0x" << std::hex << to_int(mi_pi) << std::endl;
    std::cout << std::endl;
    uint64_t one_billion = 1000000000;
    auto my_pi = pi_Leibniz_formula(one_billion);
    std::cout << std::setprecision(16) << my_pi << std::endl;
    std::cout << "0x" << std::hex << to_int(my_pi) << std::endl;
    std::cout << std::endl;
    auto std_pi = std::numbers::pi_v<double>;
    std::cout << std::setprecision(16) << std_pi << std::endl;
    std::cout << "0x" << std::hex << to_int(std_pi) << std::endl;

    return 0;
}