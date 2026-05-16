#ifndef LEXER_HPP
#define LEXER_HPP

#include "chetona.hpp"
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

namespace chetona {

class Lexer {
private:
    std::string source;
    size_t pos;
    int line;
    int column;
    
    std::unordered_map<std::string, TokenType> keywords;
    
    void initKeywords() {
        keywords["aromvho"] = TokenType::AROMVHO;
        keywords["shomapti"] = TokenType::SHOMAPTI;
        keywords["dhoro"] = TokenType::DHORO;
        keywords["var"] = TokenType::VAR;
        keywords["shorto"] = TokenType::SHORTO;
        keywords["natuva"] = TokenType::NATUVA;
        keywords["nahole"] = TokenType::NAHOLE;
        keywords["onno_thay"] = TokenType::ONNO_THAY;
        keywords["ghurnon"] = TokenType::GHURNON;
        keywords["jotokhon"] = TokenType::JOTOKHON;
        keywords["shotto"] = TokenType::SHOTTO;
        keywords["mithya"] = TokenType::MITHYA;
        keywords["shonkolon"] = TokenType::SHONKOLON;
        keywords["biyojon"] = TokenType::BIYOJON;
        keywords["gunon"] = TokenType::GUNON;
        keywords["vabhon"] = TokenType::VABHON;
        keywords["mod"] = TokenType::MOD;
        keywords["shomota"] = TokenType::SHOMOTA;
        keywords["boro"] = TokenType::BORO;
        keywords["choto"] = TokenType::CHOTO;
        keywords["prodorshon"] = TokenType::PRODORSHON;
        keywords["grohon"] = TokenType::GROHON;
        keywords["grohon_shobdo"] = TokenType::GROHON_SHOBDO;
        keywords["grohon_shonkhya"] = TokenType::GROHON_SHONKHYA;
    }

    char peek() {
        if (pos >= source.length()) return '\0';
        return source[pos];
    }
    
    char peekNext() {
        if (pos + 1 >= source.length()) return '\0';
        return source[pos + 1];
    }
    
    char advance() {
        char c = source[pos];
        pos++;
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        return c;
    }
    
    void skipWhitespace() {
        while (std::isspace(peek())) advance();
    }
    
    void skipComment() {
        if (peek() == '/' && peekNext() == '/') {
            while (peek() != '\n' && peek() != '\0') advance();
        }
    }
    
    Token readNumber() {
        std::string value;
        int startLine = line;
        int startCol = column;
        
        while (std::isdigit(peek()) || peek() == '.') {
            value += advance();
        }
        
        return Token(TokenType::NUMBER, value, startLine, startCol);
    }
    
    Token readString() {
        int startLine = line;
        int startCol = column;
        advance(); // skip opening quote
        std::string value;
        
        while (peek() != '"' && peek() != '\0') {
            value += advance();
        }
        
        if (peek() == '"') advance(); // skip closing quote
        
        return Token(TokenType::STRING, value, startLine, startCol);
    }
    
    Token readIdentifier() {
        std::string value;
        int startLine = line;
        int startCol = column;
        
        while (std::isalnum(peek()) || peek() == '_') {
            value += advance();
        }
        
        auto it = keywords.find(value);
        if (it != keywords.end()) {
            return Token(it->second, value, startLine, startCol);
        }
        
        return Token(TokenType::IDENTIFIER, value, startLine, startCol);
    }

public:
    Lexer(const std::string& src) : source(src), pos(0), line(1), column(1) {
        initKeywords();
    }
    
    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        
        while (pos < source.length()) {
            skipWhitespace();
            skipComment();
            
            if (pos >= source.length()) break;
            
            char c = peek();
            int startLine = line;
            int startCol = column;
            
            if (std::isdigit(c)) {
                tokens.push_back(readNumber());
            } else if (c == '"') {
                tokens.push_back(readString());
            } else if (std::isalpha(c) || c == '_') {
                tokens.push_back(readIdentifier());
            } else {
                // Operators and delimiters
                switch (c) {
                    case '+': advance(); tokens.push_back(Token(TokenType::PLUS, "+", startLine, startCol)); break;
                    case '-': advance(); tokens.push_back(Token(TokenType::MINUS, "-", startLine, startCol)); break;
                    case '*': advance(); tokens.push_back(Token(TokenType::MULTIPLY, "*", startLine, startCol)); break;
                    case '/': advance(); tokens.push_back(Token(TokenType::DIVIDE, "/", startLine, startCol)); break;
                    case '%': advance(); tokens.push_back(Token(TokenType::MODULO, "%", startLine, startCol)); break;
                    case '=':
                        advance();
                        if (peek() == '=') {
                            advance();
                            tokens.push_back(Token(TokenType::EQUAL, "==", startLine, startCol));
                        } else {
                            tokens.push_back(Token(TokenType::ASSIGN, "=", startLine, startCol));
                        }
                        break;
                    case '!':
                        advance();
                        if (peek() == '=') {
                            advance();
                            tokens.push_back(Token(TokenType::NOT_EQUAL, "!=", startLine, startCol));
                        }
                        break;
                    case '>':
                        advance();
                        if (peek() == '=') {
                            advance();
                            tokens.push_back(Token(TokenType::GREATER_EQUAL, ">=", startLine, startCol));
                        } else {
                            tokens.push_back(Token(TokenType::GREATER, ">", startLine, startCol));
                        }
                        break;
                    case '<':
                        advance();
                        if (peek() == '=') {
                            advance();
                            tokens.push_back(Token(TokenType::LESS_EQUAL, "<=", startLine, startCol));
                        } else {
                            tokens.push_back(Token(TokenType::LESS, "<", startLine, startCol));
                        }
                        break;
                    case '(': advance(); tokens.push_back(Token(TokenType::LPAREN, "(", startLine, startCol)); break;
                    case ')': advance(); tokens.push_back(Token(TokenType::RPAREN, ")", startLine, startCol)); break;
                    case '{': advance(); tokens.push_back(Token(TokenType::LBRACE, "{", startLine, startCol)); break;
                    case '}': advance(); tokens.push_back(Token(TokenType::RBRACE, "}", startLine, startCol)); break;
                    case ';': advance(); tokens.push_back(Token(TokenType::SEMICOLON, ";", startLine, startCol)); break;
                    case ',': advance(); tokens.push_back(Token(TokenType::COMMA, ",", startLine, startCol)); break;
                    default:
                        advance();
                        tokens.push_back(Token(TokenType::UNKNOWN, std::string(1, c), startLine, startCol));
                        break;
                }
            }
        }
        
        tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column));
        return tokens;
    }
    
    // Static method for token type to string conversion
    static std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::AROMVHO: return "AROMVHO";
            case TokenType::SHOMAPTI: return "SHOMAPTI";
            case TokenType::DHORO: return "DHORO";
            case TokenType::VAR: return "VAR";
            case TokenType::SHORTO: return "SHORTO";
            case TokenType::NATUVA: return "NATUVA";
            case TokenType::NAHOLE: return "NAHOLE";
            case TokenType::ONNO_THAY: return "ONNO_THAY";
            case TokenType::GHURNON: return "GHURNON";
            case TokenType::JOTOKHON: return "JOTOKHON";
            case TokenType::SHOTTO: return "SHOTTO";
            case TokenType::MITHYA: return "MITHYA";
            case TokenType::SHONKOLON: return "SHONKOLON";
            case TokenType::BIYOJON: return "BIYOJON";
            case TokenType::GUNON: return "GUNON";
            case TokenType::VABHON: return "VABHON";
            case TokenType::MOD: return "MOD";
            case TokenType::SHOMOTA: return "SHOMOTA";
            case TokenType::BORO: return "BORO";
            case TokenType::CHOTO: return "CHOTO";
            case TokenType::PRODORSHON: return "PRODORSHON";
            case TokenType::GROHON: return "GROHON";
            case TokenType::GROHON_SHOBDO: return "GROHON_SHOBDO";
            case TokenType::GROHON_SHONKHYA: return "GROHON_SHONKHYA";
            case TokenType::NUMBER: return "NUMBER";
            case TokenType::STRING: return "STRING";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::PLUS: return "PLUS";
            case TokenType::MINUS: return "MINUS";
            case TokenType::MULTIPLY: return "MULTIPLY";
            case TokenType::DIVIDE: return "DIVIDE";
            case TokenType::MODULO: return "MODULO";
            case TokenType::ASSIGN: return "ASSIGN";
            case TokenType::EQUAL: return "EQUAL";
            case TokenType::NOT_EQUAL: return "NOT_EQUAL";
            case TokenType::GREATER: return "GREATER";
            case TokenType::LESS: return "LESS";
            case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
            case TokenType::LESS_EQUAL: return "LESS_EQUAL";
            case TokenType::LPAREN: return "LPAREN";
            case TokenType::RPAREN: return "RPAREN";
            case TokenType::LBRACE: return "LBRACE";
            case TokenType::RBRACE: return "RBRACE";
            case TokenType::SEMICOLON: return "SEMICOLON";
            case TokenType::COMMA: return "COMMA";
            case TokenType::END_OF_FILE: return "END_OF_FILE";
            case TokenType::UNKNOWN: return "UNKNOWN";
            default: return "UNKNOWN";
        }
    }
};

} // namespace chetona

#endif // LEXER_HPP
