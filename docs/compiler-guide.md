# Compiler Architecture

## Overview

The Chetona compiler consists of four main components:

1. **Lexer** - Tokenizes source code
2. **Parser** - Builds Abstract Syntax Tree (AST)
3. **Symbol Table** - Tracks variables and scopes
4. **Code Generator** - Produces C++ output

## Lexer (lexer.hpp)

The lexer transforms source code into tokens.

### Token Types

```cpp
enum class TokenType {
    // Keywords
    AROMVHO, SHOMAPTI, DHORO, VAR,
    SHORTO, NATUVA, NAHOLE, ONNO_THAY,
    GHURNON, JOTOKHON, SHOTTO, MITHYA,
    
    // Functions
    SHONKOLON, BIYOJON, GUNON, VABHON, MOD,
    SHOMOTA, BORO, CHOTO,
    PRODORSHON, GROHON, GROHON_SHOBDO, GROHON_SHONKHYA,
    
    // Literals
    NUMBER, STRING, IDENTIFIER,
    
    // Operators & Delimiters
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    ASSIGN, EQUAL, NOT_EQUAL, GREATER, LESS,
    LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON,
    
    END_OF_FILE, UNKNOWN
};
```

### Usage

```cpp
chetona::Lexer lexer(sourceCode);
std::vector<Token> tokens = lexer.tokenize();
```

## Parser (parser.hpp)

The parser builds an AST from tokens.

### AST Node Types

```cpp
enum class NodeType {
    PROGRAM,
    MAIN_FUNCTION,
    BLOCK,
    VARIABLE_DECLARATION,
    ASSIGNMENT,
    IF_STATEMENT,
    WHILE_LOOP,
    FOR_LOOP,
    PRINT_STATEMENT,
    INPUT_STATEMENT,
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    LITERAL,
    IDENTIFIER,
    FUNCTION_CALL
};
```

### Usage

```cpp
chetona::Parser parser(tokens);
std::shared_ptr<ASTNode> ast = parser.parse();
```

## Symbol Table (symbol_table.hpp)

Tracks variable declarations and scopes.

### Usage

```cpp
chetona::SymbolTable symTable;
symTable.enterScope();
symTable.declare("x", "auto");
symTable.exitScope();
```

## Code Generator (codegen.hpp)

Generates C++ code from AST.

### Usage

```cpp
chetona::CodeGenerator codegen;
std::string cppCode = codegen.generate(ast);
```

## Compilation Pipeline

```
Source Code (.chetona)
        ↓
      Lexer
        ↓
     Tokens
        ↓
      Parser
        ↓
       AST
        ↓
  Code Generator
        ↓
   C++ Code (.cpp)
        ↓
    g++ Compiler
        ↓
  Executable
```
