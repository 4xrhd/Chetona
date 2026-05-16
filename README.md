# Chetona Compiler v2.0

Chetona is a Bengali-keyword programming language that compiles `.chetona` source into C++.

## What this project includes

- Main compiler binary: `build/chetona`
- Flex-based standalone lexer binary: `build/chetona_lexer`
- End-to-end pipeline: Lexer -> Parser -> AST -> C++ Code Generator
- Sample programs in `examples/`
- Test suite in `tests/test_compiler.cpp`
- Build systems for Linux/macOS/Windows (`Makefile`, `build.sh`, `build.bat`)

## Repository layout

```text
chetona-v2/
├── src/
│   └── main.cpp                 # Compiler CLI entry point
├── include/
│   ├── chetona.hpp              # Tokens, AST nodes, compatibility macros
│   ├── lexer.hpp                # Handwritten lexer
│   ├── parser.hpp               # Recursive-descent parser
│   ├── codegen.hpp              # C++ code generator
│   └── symbol_table.hpp         # Symbol table utility
├── tests/
│   └── test_compiler.cpp        # Unit/integration style tests
├── examples/                    # Example .chetona programs
├── docs/                        # Extra guides/reference
├── chetona.l                    # Flex lexer definition
├── Makefile                     # Cross-platform build targets
├── build.sh                     # Linux/macOS build script
└── build.bat                    # Windows build script
```

## Requirements

- `g++` with C++11 support
- `flex` (for `chetona_lexer`)
- `make` (if using the Makefile)

Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential flex make
```

## Quick start

```bash
make
./build/chetona examples/hello.chetona -o output.cpp -c
./output
```

## Build commands

Using `Makefile`:

```bash
make                # compiler + flex lexer
make compiler       # only compiler
make lexer          # only flex lexer
make test           # build and run tests
make clean          # remove build artifacts
```

Using scripts:

- Linux/macOS: `./build.sh <target>`
- Windows: `build.bat <target>`

Common script targets: `all`, `compiler`, `lexer`, `test`, `examples`, `compile`, `lex`, `run`, `clean`.

## Compiler CLI

```text
Usage: chetona <input.chetona> [options]

Options:
  -o <file>    Specify output file (default: output.cpp)
  -t           Print tokens after lexing
  -a           Print AST after parsing
  -s           Print symbol table
  -c           Compile generated C++ to executable (requires g++)
  -h, --help   Show help message
```

Examples:

```bash
./build/chetona examples/loops.chetona -o loops.cpp
./build/chetona examples/math.chetona -t -a
./build/chetona examples/hello.chetona -o hello.cpp -c
```

## Language snapshot

Program entry/exit:

- `aromvho() { ... }` -> `int main() { ... }`
- `shomapti();` -> `return 0;`

Variables:

- `dhoro x = 10;`
- `var name = "abc";`

Control flow:

- `shorto (...) { ... }`
- `natuva (...) { ... }` / `nahole (...) { ... }`
- `onno_thay { ... }`
- `ghurnon (...) { ... }` (while)
- `jotokhon (...) { ... }` (for)

Booleans:

- `shotto`, `mithya`

I/O and built-ins:

- `prodorshon(x)`
- `grohon_shobdo()`
- `grohon_shonkhya()`
- `shonkolon(a,b)`, `biyojon(a,b)`, `gunon(a,b)`, `vabhon(a,b)`, `mod(a,b)`
- `shomota(a,b)`, `boro(a,b)`, `choto(a,b)`

## Example

Chetona:

```chetona
aromvho() {
    dhoro x = 5;
    dhoro y = shonkolon(x, 3);
    shorto (boro(y, 6)) {
        prodorshon("y is greater than 6");
    } onno_thay {
        prodorshon("y is 6 or less");
    }
    shomapti();
}
```

## Testing

```bash
make test
```

Test coverage currently validates lexer tokenization, parser output, code generation, symbol table basics, and full pipeline sanity.

## Current limitations (important)

- The parser accepts only `aromvho()` as a top-level function.
- `prodorshon(...)` supports one expression argument.
- String literal generation is currently fragile in some cases (lexer stores string content without quotes).
- `-s` prints an empty/default symbol table instance; semantic population is not yet integrated into parsing/codegen.
- `chetona_lexer` (Flex) is a standalone token-dump utility and is separate from the compiler's handwritten lexer.

## Extra docs

- `docs/building.md`
- `docs/language-reference.md`
- `docs/compiler-guide.md`

## License

This project is based on the CHETONA project proposal.
