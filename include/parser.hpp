#ifndef PARSER_HPP
#define PARSER_HPP

#include "chetona.hpp"
#include "lexer.hpp"
#include <memory>
#include <stdexcept>

namespace chetona {

class ParserError : public std::runtime_error {
public:
    ParserError(const std::string& msg, int line, int col)
        : std::runtime_error("Parser Error at line " + std::to_string(line) + 
                            ", column " + std::to_string(col) + ": " + msg) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    
    Token peek() {
        return tokens[current];
    }
    
    Token previous() {
        return tokens[current - 1];
    }
    
    Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }
    
    bool isAtEnd() {
        return peek().type == TokenType::END_OF_FILE;
    }
    
    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }
    
    bool match(std::initializer_list<TokenType> types) {
        for (TokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }
    
    Token expect(TokenType type, const std::string& message) {
        if (check(type)) return advance();
        throw ParserError(message, peek().line, peek().column);
    }
    
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
    std::shared_ptr<ASTNode> parsePrimary();
    std::shared_ptr<ASTNode> parseComparison();
    std::shared_ptr<ASTNode> parseEquality();
    std::shared_ptr<ASTNode> parseAssignment();
    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseIfStatement();
    std::shared_ptr<ASTNode> parseWhileLoop();
    std::shared_ptr<ASTNode> parseForLoop();
    std::shared_ptr<ASTNode> parseVariableDeclaration();
    std::shared_ptr<ASTNode> parsePrintStatement();
    std::shared_ptr<ASTNode> parseFunctionCall();
    std::shared_ptr<ASTNode> parseMainFunction();

public:
    Parser(const std::vector<Token>& toks) : tokens(toks), current(0) {}
    
    std::shared_ptr<ASTNode> parse() {
        auto program = std::make_shared<ASTNode>(NodeType::PROGRAM, "program");
        
        while (!isAtEnd()) {
            if (check(TokenType::AROMVHO)) {
                program->children.push_back(parseMainFunction());
            } else {
                advance(); // skip unknown tokens
            }
        }
        
        return program;
    }
};

std::shared_ptr<ASTNode> Parser::parseMainFunction() {
    expect(TokenType::AROMVHO, "Expected 'aromvho'");
    expect(TokenType::LPAREN, "Expected '(' after 'aromvho'");
    expect(TokenType::RPAREN, "Expected ')' after '('");
    
    auto node = std::make_shared<ASTNode>(NodeType::MAIN_FUNCTION, "main");
    node->children.push_back(parseBlock());
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parseBlock() {
    expect(TokenType::LBRACE, "Expected '{'");
    
    auto block = std::make_shared<ASTNode>(NodeType::BLOCK, "block");
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        block->children.push_back(parseStatement());
    }
    
    expect(TokenType::RBRACE, "Expected '}'");
    return block;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::DHORO) || check(TokenType::VAR)) {
        return parseVariableDeclaration();
    }
    if (check(TokenType::SHORTO)) {
        return parseIfStatement();
    }
    if (check(TokenType::GHURNON)) {
        return parseWhileLoop();
    }
    if (check(TokenType::JOTOKHON)) {
        return parseForLoop();
    }
    if (check(TokenType::PRODORSHON)) {
        return parsePrintStatement();
    }
    if (check(TokenType::SHOMAPTI)) {
        advance();
        expect(TokenType::LPAREN, "Expected '(' after 'shomapti'");
        expect(TokenType::RPAREN, "Expected ')' after '('");
        expect(TokenType::SEMICOLON, "Expected ';' after 'shomapti'");
        return std::make_shared<ASTNode>(NodeType::LITERAL, "return");
    }
    if (check(TokenType::IDENTIFIER)) {
        return parseAssignment();
    }
    
    throw ParserError("Unexpected token: " + peek().value, peek().line, peek().column);
}

std::shared_ptr<ASTNode> Parser::parseVariableDeclaration() {
    advance(); // consume dhoro/var
    
    Token name = expect(TokenType::IDENTIFIER, "Expected variable name");
    
    expect(TokenType::ASSIGN, "Expected '=' after variable name");
    
    auto value = parseExpression();
    
    expect(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    
    auto node = std::make_shared<ASTNode>(NodeType::VARIABLE_DECLARATION, name.value);
    node->children.push_back(value);
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parseAssignment() {
    Token name = advance();
    
    expect(TokenType::ASSIGN, "Expected '=' after variable name");
    
    auto value = parseExpression();
    
    expect(TokenType::SEMICOLON, "Expected ';' after assignment");
    
    auto node = std::make_shared<ASTNode>(NodeType::ASSIGNMENT, name.value);
    node->children.push_back(value);
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parseIfStatement() {
    advance(); // consume shorto
    
    expect(TokenType::LPAREN, "Expected '(' after 'shorto'");
    auto condition = parseExpression();
    expect(TokenType::RPAREN, "Expected ')' after condition");
    
    auto thenBlock = parseBlock();
    
    auto node = std::make_shared<ASTNode>(NodeType::IF_STATEMENT, "if");
    node->children.push_back(condition);
    node->children.push_back(thenBlock);
    
    // Handle else if and else
    while (check(TokenType::NATUVA) || check(TokenType::NAHOLE)) {
        advance();
        expect(TokenType::LPAREN, "Expected '(' after 'natuva/nahole'");
        auto elseIfCondition = parseExpression();
        expect(TokenType::RPAREN, "Expected ')' after condition");
        auto elseIfBlock = parseBlock();
        
        auto elseIfNode = std::make_shared<ASTNode>(NodeType::IF_STATEMENT, "elseif");
        elseIfNode->children.push_back(elseIfCondition);
        elseIfNode->children.push_back(elseIfBlock);
        node->children.push_back(elseIfNode);
    }
    
    if (check(TokenType::ONNO_THAY)) {
        advance();
        auto elseBlock = parseBlock();
        node->children.push_back(elseBlock);
    }
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parseWhileLoop() {
    advance(); // consume ghurnon
    
    expect(TokenType::LPAREN, "Expected '(' after 'ghurnon'");
    auto condition = parseExpression();
    expect(TokenType::RPAREN, "Expected ')' after condition");
    
    auto block = parseBlock();
    
    auto node = std::make_shared<ASTNode>(NodeType::WHILE_LOOP, "while");
    node->children.push_back(condition);
    node->children.push_back(block);
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parseForLoop() {
    advance(); // consume jotokhon
    
    expect(TokenType::LPAREN, "Expected '(' after 'jotokhon'");
    
    // Initialization
    auto init = parseVariableDeclaration();
    
    // Condition
    auto condition = parseExpression();
    expect(TokenType::SEMICOLON, "Expected ';' after condition");
    
    // Update
    auto update = parseAssignment();
    expect(TokenType::RPAREN, "Expected ')' after update");
    
    auto block = parseBlock();
    
    auto node = std::make_shared<ASTNode>(NodeType::FOR_LOOP, "for");
    node->children.push_back(init);
    node->children.push_back(condition);
    node->children.push_back(update);
    node->children.push_back(block);
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parsePrintStatement() {
    advance(); // consume prodorshon
    
    expect(TokenType::LPAREN, "Expected '(' after 'prodorshon'");
    auto expr = parseExpression();
    expect(TokenType::RPAREN, "Expected ')' after expression");
    expect(TokenType::SEMICOLON, "Expected ';' after 'prodorshon'");
    
    auto node = std::make_shared<ASTNode>(NodeType::PRINT_STATEMENT, "print");
    node->children.push_back(expr);
    
    return node;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::shared_ptr<ASTNode> Parser::parseAssignment() {
    auto expr = parseEquality();
    
    if (match({TokenType::ASSIGN})) {
        auto value = parseAssignment();
        auto node = std::make_shared<ASTNode>(NodeType::ASSIGNMENT, expr->value);
        node->children.push_back(value);
        return node;
    }
    
    return expr;
}

std::shared_ptr<ASTNode> Parser::parseEquality() {
    auto expr = parseComparison();
    
    while (match({TokenType::EQUAL, TokenType::NOT_EQUAL})) {
        std::string op = previous().value;
        auto right = parseComparison();
        
        auto node = std::make_shared<ASTNode>(NodeType::BINARY_EXPRESSION, op);
        node->children.push_back(expr);
        node->children.push_back(right);
        expr = node;
    }
    
    return expr;
}

std::shared_ptr<ASTNode> Parser::parseComparison() {
    auto expr = parseTerm();
    
    while (match({TokenType::GREATER, TokenType::LESS, 
                  TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL,
                  TokenType::BORO, TokenType::CHOTO, TokenType::SHOMOTA})) {
        std::string op = previous().value;
        auto right = parseTerm();
        
        auto node = std::make_shared<ASTNode>(NodeType::BINARY_EXPRESSION, op);
        node->children.push_back(expr);
        node->children.push_back(right);
        expr = node;
    }
    
    return expr;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto expr = parseFactor();
    
    while (match({TokenType::PLUS, TokenType::MINUS,
                  TokenType::SHONKOLON, TokenType::BIYOJON})) {
        std::string op = previous().value;
        auto right = parseFactor();
        
        auto node = std::make_shared<ASTNode>(NodeType::BINARY_EXPRESSION, op);
        node->children.push_back(expr);
        node->children.push_back(right);
        expr = node;
    }
    
    return expr;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
    auto expr = parsePrimary();
    
    while (match({TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULO,
                  TokenType::GUNON, TokenType::VABHON, TokenType::MOD})) {
        std::string op = previous().value;
        auto right = parsePrimary();
        
        auto node = std::make_shared<ASTNode>(NodeType::BINARY_EXPRESSION, op);
        node->children.push_back(expr);
        node->children.push_back(right);
        expr = node;
    }
    
    return expr;
}

std::shared_ptr<ASTNode> Parser::parsePrimary() {
    if (match({TokenType::NUMBER})) {
        return std::make_shared<ASTNode>(NodeType::LITERAL, previous().value);
    }
    
    if (match({TokenType::STRING})) {
        return std::make_shared<ASTNode>(NodeType::LITERAL, previous().value);
    }
    
    if (match({TokenType::SHOTTO})) {
        return std::make_shared<ASTNode>(NodeType::LITERAL, "true");
    }
    
    if (match({TokenType::MITHYA})) {
        return std::make_shared<ASTNode>(NodeType::LITERAL, "false");
    }
    
    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<ASTNode>(NodeType::IDENTIFIER, previous().value);
    }
    
    if (match({TokenType::GROHON_SHOBDO})) {
        expect(TokenType::LPAREN, "Expected '(' after 'grohon_shobdo'");
        expect(TokenType::RPAREN, "Expected ')' after '('");
        return std::make_shared<ASTNode>(NodeType::FUNCTION_CALL, "grohon_shobdo");
    }
    
    if (match({TokenType::GROHON_SHONKHYA})) {
        expect(TokenType::LPAREN, "Expected '(' after 'grohon_shonkhya'");
        expect(TokenType::RPAREN, "Expected ')' after '('");
        return std::make_shared<ASTNode>(NodeType::FUNCTION_CALL, "grohon_shonkhya");
    }
    
    if (match({TokenType::LPAREN})) {
        auto expr = parseExpression();
        expect(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    throw ParserError("Unexpected token in expression: " + peek().value, peek().line, peek().column);
}

std::shared_ptr<ASTNode> Parser::parseFunctionCall() {
    Token name = advance();
    expect(TokenType::LPAREN, "Expected '(' after function name");
    
    auto node = std::make_shared<ASTNode>(NodeType::FUNCTION_CALL, name.value);
    
    if (!check(TokenType::RPAREN)) {
        do {
            node->children.push_back(parseExpression());
        } while (match({TokenType::COMMA}));
    }
    
    expect(TokenType::RPAREN, "Expected ')' after arguments");
    return node;
}

} // namespace chetona

#endif // PARSER_HPP
