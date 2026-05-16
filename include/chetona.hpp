#ifndef CHETONA_HPP
#define CHETONA_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace chetona {

// Token types for the lexer
enum class TokenType {
    // Keywords
    AROMVHO,        // main
    SHOMAPTI,       // return 0
    DHORO,          // variable declaration
    VAR,            // variable declaration (alias)
    SHORTO,         // if
    NATUVA,         // else if
    NAHOLE,         // else if (alternative)
    ONNO_THAY,      // else
    GHURNON,        // while
    JOTOKHON,       // for
    SHOTTO,         // true
    MITHYA,         // false
    
    // Functions
    SHONKOLON,      // addition
    BIYOJON,        // subtraction
    GUNON,          // multiplication
    VABHON,         // division
    MOD,            // modulo
    SHOMOTA,        // equal
    BORO,           // greater than
    CHOTO,          // less than
    PRODORSHON,     // print
    GROHON,         // input
    GROHON_SHOBDO,  // string input
    GROHON_SHONKHYA, // number input
    
    // Literals
    NUMBER,
    STRING,
    IDENTIFIER,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGN,
    EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    
    // Delimiters
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COMMA,
    
    // Special
    END_OF_FILE,
    UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l = 0, int c = 0)
        : type(t), value(v), line(l), column(c) {}
};

// Symbol table entry
struct Symbol {
    std::string name;
    std::string type;
    int scope;
    bool isInitialized;
    
    Symbol() : name(""), type(""), scope(0), isInitialized(false) {}
    Symbol(const std::string& n, const std::string& t, int s = 0)
        : name(n), type(t), scope(s), isInitialized(false) {}
};

// AST Node types
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

// AST Node
struct ASTNode {
    NodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;
    int line;
    
    ASTNode(NodeType t, const std::string& v = "", int l = 0)
        : type(t), value(v), line(l) {}
};

} // namespace chetona

// Macros for backward compatibility
#define aromvho() int main()
#define shomapti() return 0
#define dhoro auto
#define var auto
#define shorto if
#define natuva else if
#define nahole else if
#define onno_thay else
#define ghurnon while
#define jotokhon for
#define shotto true
#define mithya false
#define shonkolon(a, b) ((a) + (b))
#define biyojon(a, b) ((a) - (b))
#define gunon(a, b) ((a) * (b))
#define vabhon(a, b) ((a) / (b))
#define mod(a, b) ((a) % (b))
#define shomota(a, b) ((a) == (b))
#define boro(a, b) ((a) > (b))
#define choto(a, b) ((a) < (b))
#define prodorshon(x) std::cout << x << std::endl
#define grohon(x) std::cin >> x

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

#endif // CHETONA_HPP
