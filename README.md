# C++ is Magic: Static and Consteval Examples

C++ is the most powerful language.
This repository shows the power and magic of modern C++ through some static and consteval examples. This project highlights, through examples, how to write more efficient, safe, and performant code by leveraging static metaprogramming and compile-time evaluations.

# Table of Contents
 - [Environments](Environments)
 - [Examples](Examples)
   - [1) Consteval Templated Tensor Class](example-1-consteval-templated-tensor-class)
 - [License](License)

### Environments
All examples using __C++2b__ and have been tested and compiled with `Clang` and `GCC` via the [Compiler Explorer](https://godbolt.org). 


# Examples
## Example 1: Consteval Templated Tensor Class
### Overview
This example demonstrates a consteval templated tensor class that performs operations at compile time. The code generates and executes tensor operations without generating any assembly code, showcasing the efficiency of compile-time computations.

### Code Example
Complied with `x86-64 clang 18.1.0` using the following options `-std=c++2b -O3 -Wall -Wextra -Wpedantic -Werror`.
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


# License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
