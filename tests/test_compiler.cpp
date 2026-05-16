#include <iostream>
#include <cassert>
#include <string>
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/codegen.hpp"
#include "../include/symbol_table.hpp"

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
    
    // Test default constructor
    Symbol s;
    assert(s.name == "");
    assert(s.isInitialized == false);
    
    // Test parameterized constructor
    Symbol s2("x", "auto", 0);
    assert(s2.name == "x");
    assert(s2.type == "auto");
    assert(s2.scope == 0);
    
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

void testFullPipeline() {
    std::cout << "Testing Full Pipeline..." << std::endl;
    
    // Test a more complex program
    std::string source = R"(
        aromvho() {
            dhoro x = 10;
            prodorshon(x);
            shomapti();
        }
    )";
    
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    assert(tokens.size() > 0);
    
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast != nullptr);
    
    CodeGenerator codegen;
    std::string code = codegen.generate(ast);
    
    assert(code.find("auto x = 10") != std::string::npos);
    assert(code.find("std::cout") != std::string::npos);
    
    std::cout << "Full Pipeline tests passed!" << std::endl;
}

int main() {
    std::cout << "\n=== Chetona Compiler Tests ===\n" << std::endl;
    
    try {
        testLexer();
        testParser();
        testCodeGenerator();
        testSymbolTable();
        testFullPipeline();
        
        std::cout << "\nAll tests passed!\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
