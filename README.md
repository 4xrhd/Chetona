# Chetona Compiler v2.0

A Bengali-based programming language compiler that translates Chetona code to C++.

## Overview

Chetona is a domain-specific language (DSL) that uses Bengali keywords to make programming more accessible to Bengali speakers. This compiler translates Chetona source code into standard C++.

## Features

- **Bengali Keywords**: Write code using familiar Bengali words
- **Full Compiler Pipeline**: Lexer → Parser → AST → Code Generator
- **C++ Output**: Generates clean, compilable C++ code
- **Symbol Table**: Tracks variable declarations and scopes
- **Multiple Output Options**: Tokens, AST, or compiled executable

## Project Structure

```
chetona-v2/
├── include/           # Header files
│   ├── chetona.hpp    # Core definitions and tokens
│   ├── lexer.hpp      # Lexical analyzer
│   ├── parser.hpp     # Parser and AST
│   ├── codegen.hpp    # Code generator
│   └── symbol_table.hpp
├── src/
│   └── main.cpp       # Compiler entry point
├── examples/          # Sample Chetona programs
├── tests/             # Test files
├── build/             # Build output
├── docs/              # Documentation
├── chetona.l          # Flex lexer definition
└── Makefile           # Build configuration
```

## Quick Start

### Build

```bash
make
```

### Compile a Chetona file

```bash
./build/chetona examples/hello.chetona -o output.cpp
```

### Compile and run

```bash
./build/chetona examples/hello.chetona -o output.cpp -c
./output
```

### Run the Flex lexer

```bash
./build/chetona_lexer examples/hello.chetona
```

## Language Reference

### Keywords

| Bengali | English | Description |
| :--- | :--- | :--- |
| `aromvho()` | `int main()` | Program entry |
| `shomapti()` | `return 0` | Program exit |
| `dhoro` | `auto` | Variable declaration |
| `var` | `auto` | Variable declaration |
| `shorto` | `if` | Conditional |
| `natuva` | `else if` | Else if |
| `nahole` | `else if` | Else if (alt) |
| `onno_thay` | `else` | Else |
| `ghurnon` | `while` | While loop |
| `jotokhon` | `for` | For loop |
| `shotto` | `true` | Boolean true |
| `mithya` | `false` | Boolean false |

### Functions

| Bengali | Description |
| :--- | :--- |
| `shonkolon(a, b)` | Addition (a + b) |
| `biyojon(a, b)` | Subtraction (a - b) |
| `gunon(a, b)` | Multiplication (a * b) |
| `vabhon(a, b)` | Division (a / b) |
| `mod(a, b)` | Modulo (a % b) |
| `shomota(a, b)` | Equal (a == b) |
| `boro(a, b)` | Greater (a > b) |
| `choto(a, b)` | Less (a < b) |
| `prodorshon(x)` | Print to console |
| `grohon_shobdo()` | Read string input |
| `grohon_shonkhya()` | Read number input |

## Compiler Options

```
Usage: chetona <input.chetona> [options]

Options:
  -o <file>    Specify output file (default: output.cpp)
  -t           Print tokens after lexing
  -a           Print AST after parsing
  -s           Print symbol table
  -c           Compile to executable (requires g++)
  -h, --help   Show help message
```

## Example

### Chetona code:

```chetona
aromvho() {
    prodorshon("Hello World in Bengali!");
    shomapti();
}
```

### Generated C++ code:

```cpp
#include <iostream>
#include <string>

inline std::string grohon_shobdo() {
    std::string s;
    std::cin >> s;
    return s;
}

inline double grohon_shonkhya() {
    double d;
    std::cin >> d;
    return d;
}

int main() {
    std::cout << "Hello World in Bengali!" << std::endl;
    return 0;
}
```

## Building from Source

### Prerequisites

- g++ (C++11 support)
- Flex
- Make

### Install on Ubuntu/Debian

```bash
sudo apt-get install build-essential flex
```

### Build

```bash
make
```

### Run tests

```bash
make test
```

## License

This project is based on the CHETONA project proposal.
