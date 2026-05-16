#include <iostream>
#include <fstream>
#include <string>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"
#include "symbol_table.hpp"

void printUsage(const std::string& programName) {
    std::cout << "Chetona Compiler v2.0\n";
    std::cout << "Usage: " << programName << " <input.chetona> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o <file>    Specify output file (default: output.cpp)\n";
    std::cout << "  -t           Print tokens after lexing\n";
    std::cout << "  -a           Print AST after parsing\n";
    std::cout << "  -s           Print symbol table\n";
    std::cout << "  -c           Compile to executable (requires g++)\n";
    std::cout << "  -h, --help   Show this help message\n";
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    return content;
}

void printTokens(const std::vector<chetona::Token>& tokens) {
    std::cout << "\n=== Tokens ===\n";
    for (const auto& token : tokens) {
        std::cout << chetona::Lexer::tokenTypeToString(token.type) 
                  << " [" << token.value << "] "
                  << "(line " << token.line << ", col " << token.column << ")\n";
    }
    std::cout << "==============\n";
}

void printAST(std::shared_ptr<chetona::ASTNode> node, int indent = 0) {
    std::string ind(indent * 2, ' ');
    
    std::cout << ind << "Node: " << node->value;
    std::cout << " (Type: " << static_cast<int>(node->type) << ")\n";
    
    for (const auto& child : node->children) {
        printAST(child, indent + 1);
    }
}

std::string nodeTypeToString(chetona::NodeType type) {
    switch (type) {
        case chetona::NodeType::PROGRAM: return "PROGRAM";
        case chetona::NodeType::MAIN_FUNCTION: return "MAIN_FUNCTION";
        case chetona::NodeType::BLOCK: return "BLOCK";
        case chetona::NodeType::VARIABLE_DECLARATION: return "VARIABLE_DECLARATION";
        case chetona::NodeType::ASSIGNMENT: return "ASSIGNMENT";
        case chetona::NodeType::IF_STATEMENT: return "IF_STATEMENT";
        case chetona::NodeType::WHILE_LOOP: return "WHILE_LOOP";
        case chetona::NodeType::FOR_LOOP: return "FOR_LOOP";
        case chetona::NodeType::PRINT_STATEMENT: return "PRINT_STATEMENT";
        case chetona::NodeType::INPUT_STATEMENT: return "INPUT_STATEMENT";
        case chetona::NodeType::BINARY_EXPRESSION: return "BINARY_EXPRESSION";
        case chetona::NodeType::UNARY_EXPRESSION: return "UNARY_EXPRESSION";
        case chetona::NodeType::LITERAL: return "LITERAL";
        case chetona::NodeType::IDENTIFIER: return "IDENTIFIER";
        case chetona::NodeType::FUNCTION_CALL: return "FUNCTION_CALL";
        default: return "UNKNOWN";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile;
    std::string outputFile = "output.cpp";
    bool printTokensFlag = false;
    bool printASTFlag = false;
    bool printSymbolTable = false;
    bool compileFlag = false;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "Error: -o requires a filename\n";
                return 1;
            }
        } else if (arg == "-t") {
            printTokensFlag = true;
        } else if (arg == "-a") {
            printASTFlag = true;
        } else if (arg == "-s") {
            printSymbolTable = true;
        } else if (arg == "-c") {
            compileFlag = true;
        } else if (arg[0] != '-') {
            inputFile = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }
    
    if (inputFile.empty()) {
        std::cerr << "Error: No input file specified\n";
        return 1;
    }
    
    // Read source file
    std::string source = readFile(inputFile);
    if (source.empty()) {
        return 1;
    }
    
    std::cout << "Compiling " << inputFile << "...\n";
    
    try {
        // Lexer
        chetona::Lexer lexer(source);
        std::vector<chetona::Token> tokens = lexer.tokenize();
        
        if (printTokensFlag) {
            printTokens(tokens);
        }
        
        // Parser
        chetona::Parser parser(tokens);
        std::shared_ptr<chetona::ASTNode> ast = parser.parse();
        
        if (printASTFlag) {
            std::cout << "\n=== AST ===\n";
            printAST(ast);
            std::cout << "===========\n";
        }
        
        // Symbol Table
        if (printSymbolTable) {
            chetona::SymbolTable symTable;
            symTable.print();
        }
        
        // Code Generation
        chetona::CodeGenerator codegen;
        std::string cppCode = codegen.generate(ast);
        
        // Save to file
        std::ofstream outFile(outputFile);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not write to '" << outputFile << "'\n";
            return 1;
        }
        outFile << cppCode;
        outFile.close();
        
        std::cout << "Generated: " << outputFile << "\n";
        
        // Compile to executable if requested
        if (compileFlag) {
            std::string execFile = outputFile.substr(0, outputFile.rfind('.'));
            std::string compileCmd = "g++ -std=c++11 " + outputFile + " -o " + execFile;
            
            std::cout << "Compiling to executable...\n";
            int result = system(compileCmd.c_str());
            
            if (result == 0) {
                std::cout << "Executable: " << execFile << "\n";
            } else {
                std::cerr << "Compilation failed\n";
                return 1;
            }
        }
        
        std::cout << "Done!\n";
        
    } catch (const chetona::ParserError& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
