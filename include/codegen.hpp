#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include "chetona.hpp"
#include "parser.hpp"
#include <sstream>
#include <fstream>

namespace chetona {

class CodeGenerator {
private:
    std::stringstream output;
    int indentLevel;
    
    std::string indent() {
        return std::string(indentLevel * 4, ' ');
    }
    
    std::string generateExpression(std::shared_ptr<ASTNode> node) {
        std::stringstream expr;
        
        switch (node->type) {
            case NodeType::LITERAL:
                if (node->value == "true") expr << "true";
                else if (node->value == "false") expr << "false";
                else if (node->value == "return") expr << "";
                else expr << node->value;
                break;
                
            case NodeType::IDENTIFIER:
                expr << node->value;
                break;
                
            case NodeType::BINARY_EXPRESSION:
                expr << "(";
                expr << generateExpression(node->children[0]);
                expr << " " << node->value << " ";
                expr << generateExpression(node->children[1]);
                expr << ")";
                break;
                
            case NodeType::FUNCTION_CALL:
                if (node->value == "grohon_shobdo") {
                    expr << "grohon_shobdo()";
                } else if (node->value == "grohon_shonkhya") {
                    expr << "grohon_shonkhya()";
                }
                break;
                
            default:
                expr << node->value;
                break;
        }
        
        return expr.str();
    }
    
    void generateStatement(std::shared_ptr<ASTNode> node) {
        switch (node->type) {
            case NodeType::VARIABLE_DECLARATION:
                output << indent() << "auto " << node->value << " = "
                       << generateExpression(node->children[0]) << ";\n";
                break;
                
            case NodeType::ASSIGNMENT:
                output << indent() << node->value << " = "
                       << generateExpression(node->children[0]) << ";\n";
                break;
                
            case NodeType::PRINT_STATEMENT:
                output << indent() << "std::cout << "
                       << generateExpression(node->children[0]) 
                       << " << std::endl;\n";
                break;
                
            case NodeType::IF_STATEMENT:
                generateIfStatement(node);
                break;
                
            case NodeType::WHILE_LOOP:
                generateWhileLoop(node);
                break;
                
            case NodeType::FOR_LOOP:
                generateForLoop(node);
                break;
                
            case NodeType::LITERAL:
                if (node->value == "return") {
                    output << indent() << "return 0;\n";
                }
                break;
                
            default:
                break;
        }
    }
    
    void generateIfStatement(std::shared_ptr<ASTNode> node) {
        output << indent() << "if (" << generateExpression(node->children[0]) << ") ";
        
        // Generate then block
        generateBlock(node->children[1]);
        
        // Generate else if and else clauses
        for (size_t i = 2; i < node->children.size(); i++) {
            if (node->children[i]->type == NodeType::IF_STATEMENT) {
                output << indent() << "else if (" 
                       << generateExpression(node->children[i]->children[0]) << ") ";
                generateBlock(node->children[i]->children[1]);
            } else {
                output << indent() << "else ";
                generateBlock(node->children[i]);
            }
        }
    }
    
    void generateWhileLoop(std::shared_ptr<ASTNode> node) {
        output << indent() << "while (" << generateExpression(node->children[0]) << ") ";
        generateBlock(node->children[1]);
    }
    
    void generateForLoop(std::shared_ptr<ASTNode> node) {
        output << indent() << "for (";
        
        // Initialization
        output << "auto " << node->children[0]->value << " = "
               << generateExpression(node->children[0]->children[0]) << "; ";
        
        // Condition
        output << generateExpression(node->children[1]) << "; ";
        
        // Update
        output << node->children[2]->value << " = "
               << generateExpression(node->children[2]->children[0]) << ") ";
        
        generateBlock(node->children[3]);
    }
    
    void generateBlock(std::shared_ptr<ASTNode> node) {
        output << "{\n";
        indentLevel++;
        
        for (auto& child : node->children) {
            generateStatement(child);
        }
        
        indentLevel--;
        output << indent() << "}\n";
    }
    
    void generateMainFunction(std::shared_ptr<ASTNode> node) {
        output << "int main() {\n";
        indentLevel++;
        
        if (!node->children.empty()) {
            generateBlock(node->children[0]);
        }
        
        indentLevel--;
        output << "}\n";
    }

public:
    CodeGenerator() : indentLevel(0) {}
    
    std::string generate(std::shared_ptr<ASTNode> ast) {
        // Include headers
        output << "#include <iostream>\n";
        output << "#include <string>\n\n";
        
        // Helper functions
        output << "inline std::string grohon_shobdo() {\n";
        output << "    std::string s;\n";
        output << "    std::cin >> s;\n";
        output << "    return s;\n";
        output << "}\n\n";
        
        output << "inline double grohon_shonkhya() {\n";
        output << "    double d;\n";
        output << "    std::cin >> d;\n";
        output << "    return d;\n";
        output << "}\n\n";
        
        // Generate main function
        for (auto& child : ast->children) {
            if (child->type == NodeType::MAIN_FUNCTION) {
                generateMainFunction(child);
            }
        }
        
        return output.str();
    }
    
    bool saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << output.str();
        file.close();
        return true;
    }
};

} // namespace chetona

#endif // CODEGEN_HPP
