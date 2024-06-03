#include <array>
#include <string>

template <typename CT, size_t S>
class Tensor {
    using T = typename std::remove_const<CT>::type;

  public:
    consteval Tensor(const CT* data = nullptr) {
        if (data != nullptr) {
            for (size_t i = 0; i < S; ++i) {
                this->data[i] = data[i];
            }
        }
    }

    consteval const T* get_data() const { return data; }

    consteval size_t size() const { return S; }

    template <typename T2, size_t S2>
    consteval auto operator+(const Tensor<T2, S2>& other) const {
        static_assert(S == S2, "Tensors must be the same size to be added.");
        std::array<T, S> sum{};
        for (size_t i = 0; i < this->size(); ++i) {
            sum[i] = this->data[i] + other.get_data()[i];
        }
        return Tensor<T, S>(sum.data());
    }

    consteval std::string_view dump() const { return std::string_view(data, S - 1); }

  private:
    T data[S] = {};
};

template <typename CT, size_t S>
Tensor(const CT (&)[S]) -> Tensor<CT, S>;

int main() {
    constexpr char encrypted_message[] = "Hello world!";
    constexpr char decryption_key[] = {-5, -58, -65, -76, -6, 83, -87, -2, -17, -5, 5, 66, 0};

    constexpr auto tensor_1 = Tensor(encrypted_message);
    constexpr auto tensor_2 = Tensor(decryption_key);
    constexpr auto tensor_3 = tensor_1 + tensor_2;

    static_assert(tensor_3.dump() == "C++ is magic");
    return 0;
}
