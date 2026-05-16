# Building Guide

## Prerequisites

- **g++** - C++ compiler with C++11 support
- **Flex** - Fast lexical analyzer generator
- **Make** - Build automation tool

## Installation

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install build-essential flex
```

### Fedora/RHEL

```bash
sudo dnf install gcc-c++ flex make
```

### macOS

```bash
brew install flex
xcode-select --install
```

## Build Commands

### Build Everything

```bash
make
```

This builds:
- `build/chetona` - Main compiler
- `build/chetona_lexer` - Flex lexer

### Build Specific Components

```bash
make compiler    # Build only the compiler
make lexer       # Build only the Flex lexer
```

### Compile a File

```bash
make compile FILE=examples/hello.chetona
```

### Run the Lexer

```bash
make lex FILE=examples/hello.chetona
```

### Run Tests

```bash
make test
```

### Build Examples

```bash
make examples
```

### Clean Build Files

```bash
make clean
```

## Compiler Usage

### Basic Compilation

```bash
./build/chetona input.chetona -o output.cpp
```

### Show Tokens

```bash
./build/chetona input.chetona -t
```

### Show AST

```bash
./build/chetona input.chetona -a
```

### Compile to Executable

```bash
./build/chetona input.chetona -o output.cpp -c
```

### Full Options

```bash
./build/chetona input.chetona -t -a -s -c
```

## Troubleshooting

### "flex: command not found"

Install Flex:
```bash
sudo apt-get install flex
```

### "g++: command not found"

Install build tools:
```bash
sudo apt-get install build-essential
```

### "C++11 features not supported"

Add flag manually:
```bash
g++ -std=c++11 src/main.cpp -o build/chetona
```
