#include <array>
#include <cstddef>

namespace hide {
template <typename T, size_t X, size_t Y>
class Matrix {
  public:
    constexpr Matrix(const std::initializer_list<T> init) {
        std::copy(init.begin(), init.end(), data.begin());
    }

    template <size_t X2, size_t Y2>
    constexpr auto dotProduct(const Matrix<T, X2, Y2>& matrice2) const {
        static_assert(Y == X2, "Incompatible dimensions.");
        Matrix<T, X, Y2> result({});
        for (size_t i = 0; i < X; ++i) {
            for (size_t j = 0; j < Y2; ++j) {
                for (size_t k = 0; k < X2; ++k) {
                    const auto idx_ij = i * Y2 + j;
                    const auto idx_ik = i * Y + k;
                    const auto idx_kj = k * Y2 + j;
                    result.data[idx_ij] += this->data[idx_ik] * matrice2.data[idx_kj];
                }
            }
        }
        return result;
    }

    constexpr auto operator==(const std::array<T, X * Y>& init) { return this->data == init; }

  public:
    std::array<T, X * Y> data{};
};
}  // namespace hide

template <size_t X, size_t Y, typename T>
constexpr hide::Matrix<T, X, Y> Matrix(std::initializer_list<T> init) {
    return hide::Matrix<T, X, Y>(init);
}

int main() {
    constexpr auto a = Matrix<1, 3>({5., 6., 7.});  // double
    constexpr auto b = Matrix<3, 1>({3., 2., 1.});
    constexpr std::array ab{34.};
    static_assert(a.dotProduct(b) == ab);

    constexpr std::array ba{15., 18., 21., 10., 12., 14., 5., 6., 7.};
    static_assert(b.dotProduct(a) == ba);

    constexpr auto c = Matrix<2, 2>({1u, 2u, 3u, 4u});  // unsigned int
    constexpr auto d = Matrix<2, 1>({8u, 9u});
    constexpr std::array cd{26u, 60u};
    static_assert(c.dotProduct(d) == cd);

    constexpr auto e = Matrix<2, 2>({1.f, 2.f, 3.f, 6.f});  // float
    constexpr auto f = Matrix<2, 2>({4.f, 7.f, 2.f, 1.f});
    constexpr std::array ef{8.f, 9.f, 24.f, 27.f};
    static_assert(e.dotProduct(f) == ef);

    constexpr auto g = Matrix<3, 3>({1, 2, 3, 4, 5, 6, 7, 8, 9});  // int
    constexpr auto h = Matrix<3, 3>({1, 2, 1, 2, 4, 6, 7, 2, 5});
    constexpr std::array gh{26, 16, 28, 56, 40, 64, 86, 64, 100};
    static_assert(g.dotProduct(h) == gh);
    return 0;
}
