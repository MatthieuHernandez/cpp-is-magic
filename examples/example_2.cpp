#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>

namespace hide {
template <typename T, size_t X, size_t Y>
class Matrix {
  public:
    constexpr Matrix(const std::initializer_list<T> init) {
        std::copy(init.begin(), init.end(), data.begin());
    }

    template <size_t X2, size_t Y2>
    auto dotProduct(const Matrix<T, X2, Y2>& matrice2) const {
        static_assert(Y == X2, "Incompatible dimensions.");
        Matrix<T, X, Y2> result({});
        for (size_t i = 0; i < X; ++i) {
            for (size_t j = 0; j < Y2; ++j) {
                for (size_t k = 0; k < X2; ++k) {
                    auto idx_ij = i * Y2 + j;
                    auto idx_ik = i * Y + k;
                    auto idx_kj = k * Y2 + j;
                    result.data[idx_ij] += this->data[idx_ik] * matrice2.data[idx_kj];
                }
            }
        }
        return result;
    }

    constexpr auto operator==(std::array<T, X * Y>& init) { return this->data == init; }

  public:
    std::array<T, X * Y> data{};
};
}  // namespace hide

template <size_t X, size_t Y, typename T>
hide::Matrix<T, X, Y> Matrix(std::initializer_list<T> init) {
    return hide::Matrix<T, X, Y>(init);
}

int main() {
    const auto a = Matrix<1, 3>({5, 6, 7});
    const auto b = Matrix<3, 1>({3, 2, 1});
    std::array ab{34};
    assert(a.dotProduct(b) == ab);

    std::array ba{15, 18, 21, 10, 12, 14, 5, 6, 7};
    assert(b.dotProduct(a) == ba);

    const auto c = Matrix<2, 2>({1, 2, 3, 4});
    const auto d = Matrix<2, 1>({8, 9});
    std::array cd{26, 60};
    assert(c.dotProduct(d) == cd);

    const auto e = Matrix<2, 2>({1, 2, 3, 6});
    const auto f = Matrix<2, 2>({4, 7, 2, 1});
    std::array ef{8, 9, 24, 27};
    assert(e.dotProduct(f) == ef);

    const auto g = Matrix<3, 3>({1, 2, 3, 4, 5, 6, 7, 8, 9});
    const auto h = Matrix<3, 3>({1, 2, 1, 2, 4, 6, 7, 2, 5});
    std::array gh{26, 16, 28, 56, 40, 64, 86, 64, 100};
    assert(g.dotProduct(h) == gh);
    return 0;
}