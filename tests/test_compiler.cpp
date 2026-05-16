#include <iostream>
#include <cassert>
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/codegen.hpp"

using namespace chetona;

void testLexer() {
    std::cout << "Testing Lexer..." << std::endl;
    
    // Test 1: Simple program
    Lexer lexer1("aromvho() { prodorshon(\"Hello\"); shomapti(); }");
    auto tokens1 = lexer1.tokenize();
    assert(tokens1.size() > 0);
    assert(tokens1[0].type == TokenType::AROMVHO);
    
    // Test 2: Numbers
    Lexer lexer2("dhoro x = 42;");
    auto tokens2 = lexer2.tokenize();
    assert(tokens2.size() > 0);
    
    // Test 3: Keywords
    Lexer lexer3("shorto ghurnon jotokhon");
    auto tokens3 = lexer3.tokenize();
    assert(tokens3[0].type == TokenType::SHORTO);
    assert(tokens3[1].type == TokenType::GHURNON);
    assert(tokens3[2].type == TokenType::JOTOKHON);
    
    std::cout << "Lexer tests passed!" << std::endl;
}

void testParser() {
    std::cout << "Testing Parser..." << std::endl;
    
    // Test simple program
    Lexer lexer("aromvho() { shomapti(); }");
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast != nullptr);
    assert(ast->type == NodeType::PROGRAM);
    
    std::cout << "Parser tests passed!" << std::endl;
}

void testCodeGenerator() {
    std::cout << "Testing Code Generator..." << std::endl;
    
    Lexer lexer("aromvho() { prodorshon(\"Hello\"); shomapti(); }");
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    CodeGenerator codegen;
    std::string code = codegen.generate(ast);
    
    assert(code.find("#include") != std::string::npos);
    assert(code.find("main()") != std::string::npos);
    
    std::cout << "Code Generator tests passed!" << std::endl;
}

void testSymbolTable() {
    std::cout << "Testing Symbol Table..." << std::endl;
    
    SymbolTable table;
    
    // Test scope management
    table.enterScope();
    assert(table.getCurrentScope() == 1);
    table.exitScope();
    assert(table.getCurrentScope() == 0);
    
    // Test variable declaration
    table.declare("x", "auto");
    Symbol* sym = table.lookup("x");
    assert(sym != nullptr);
    assert(sym->name == "x");
    
    std::cout << "Symbol Table tests passed!" << std::endl;
}

int main() {
    std::cout << "\n=== Chetona Compiler Tests ===\n" << std::endl;
    
    testLexer();
    testParser();
    testCodeGenerator();
    testSymbolTable();
    
    std::cout << "\nAll tests passed!\n" << std::endl;
    return 0;
}
