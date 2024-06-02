# C++ is Magic: Compile-time Examples

C++ is the most powerful language.
This repository highlights, through examples, how to write more efficient, safe, and performant modern C++ code by leveraging static metaprogramming and compile-time evaluations.

# Table of Contents
 - [Environment](#environment)
 - [Examples](#example)
   - [1) Consteval Templated Tensor Class](#example-1)
 - [License](#license)

### Environment <a name="environment"></a>
All examples using at least __C++20__ and have been tested and compiled with `Clang` and `GCC` via the [Compiler Explorer](https://godbolt.org). 


# Examples <a name="example"></a>
## Example 1: Consteval Templated Tensor Class <a name="example-1"></a>
__[example_1.cpp](examples/example_1.cpp)__
### Overview
This example showcasing the efficiency of compile-time computations by creating tensor objects via a consteval templated class and performs an addition of 2 tensors of different types at compile time.

### Code Example

Complied with `x86-64 clang 18.1.0` using the following options `-std=c++2a -O3 -Wall -Wextra -Wpedantic -Werror`.
```cpp
int main() {
    static constexpr char encrypted_message[] = "Hello world!";
    static constexpr char decryption_key[] = {-5, -58, -65, -76, -6, 83, -87, -2, -17, -5, 5, 66, 0};
    
    static constexpr auto tensor_1 = Tensor(encrypted_message);
    static constexpr auto tensor_2 = Tensor(decryption_key);
    static constexpr auto tensor_3 = tensor_1 + tensor_2;

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


# License <a name="license"></a>
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
