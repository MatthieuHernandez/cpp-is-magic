# C++ is Magic: Compile-time Examples

C++ is the most powerful language.
This repository highlights, through examples, how to write more efficient, safe, and performant modern C++ code by leveraging static metaprogramming and compile-time evaluations.

# Table of Contents
 - [Environment](#environment)
 - [Examples](#example)
   - [1) Compile-time templated tensor class](#example-1)
     - With const and non-const type handling in the template.
   - [2) Compile-time dot product of matrices](#example-2)
     - Including a trick to partially deduce the template type.
 - [License](#license)

### Environment <a name="environment"></a>
All examples using at least __C++20__ and have been tested and compiled with `Clang` and `GCC` via the [Compiler Explorer](https://godbolt.org). 


# Examples <a name="example"></a>
## Example 1: Compile-time templated tensor class <a name="example-1"></a>
__[example_1.cpp](examples/example_1.cpp)__
### Overview
This example showcasing the efficiency of compile-time computations by creating tensor objects via a templated class and performs an addition of 2 tensors of different types at compile-time.

### Code Example

Complied with `x86-64 clang 18.1.0` using the following options `-std=c++23 -O3 -Wall -Wextra -Wpedantic -Werror`.
```cpp
int main() {
    constexpr char encrypted_message[] = "Hello world!";
    constexpr int decryption_key[] = {-5, -58, -65, -76, -6, 83, -87, -2, -17, -5, 5, 66, 0};

    constexpr auto tensor_1 = Tensor(encrypted_message);
    constexpr auto tensor_2 = Tensor(decryption_key);
    constexpr auto tensor_3 = tensor_1 + tensor_2;

    static_assert(tensor_3.dump() == "C++ is magic");
    return 0;
}
```
This code generates and executes tensor operators without generating any assembly code.
```assembly
main:
    xor    eax, eax
    ret
```

## Example 2: Compile-time dot product of matrices <a name="example-2"></a>
__[example_2.cpp](examples/example_2.cpp)__
### Overview
This example showcasing the efficiency of compile-time computations by creating matrixes via a templated class and performs a dot product at compile-time.

### Code Example

Complied with `x86-64 clang 18.1.0` using the following options `-std=c++23 -O3 -Wall -Wextra -Wpedantic -Werror`.
```cpp
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
```
This code performs dot products without generating any assembly code.
```assembly
main:
    xor    eax, eax
    ret
```


# License <a name="license"></a>
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
