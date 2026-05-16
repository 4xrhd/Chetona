#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "chetona.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace chetona {

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    int currentScope;
    
public:
    SymbolTable() : currentScope(0) {
        scopes.push_back(std::unordered_map<std::string, Symbol>());
    }
    
    void enterScope() {
        currentScope++;
        scopes.push_back(std::unordered_map<std::string, Symbol>());
    }
    
    void exitScope() {
        if (currentScope > 0) {
            scopes.pop_back();
            currentScope--;
        }
    }
    
    bool declare(const std::string& name, const std::string& type) {
        if (scopes[currentScope].find(name) != scopes[currentScope].end()) {
            return false; // Already declared in current scope
        }
        
        scopes[currentScope][name] = Symbol(name, type, currentScope);
        return true;
    }
    
    Symbol* lookup(const std::string& name) {
        for (int i = currentScope; i >= 0; i--) {
            auto it = scopes[i].find(name);
            if (it != scopes[i].end()) {
                return &it->second;
            }
        }
        return nullptr; // Not found
    }
    
    bool isInCurrentScope(const std::string& name) {
        return scopes[currentScope].find(name) != scopes[currentScope].end();
    }
    
    void setInitialized(const std::string& name) {
        Symbol* sym = lookup(name);
        if (sym) {
            sym->isInitialized = true;
        }
    }
    
    void print() {
        std::cout << "=== Symbol Table ===\n";
        for (int i = 0; i <= currentScope; i++) {
            std::cout << "Scope " << i << ":\n";
            for (auto& pair : scopes[i]) {
                std::cout << "  " << pair.second.name << " : " << pair.second.type;
                if (pair.second.isInitialized) {
                    std::cout << " (initialized)";
                }
                std::cout << "\n";
            }
        }
        std::cout << "====================\n";
    }
    
    int getCurrentScope() const {
        return currentScope;
    }
};

} // namespace chetona

#endif // SYMBOL_TABLE_HPP
