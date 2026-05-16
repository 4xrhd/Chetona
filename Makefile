# Chetona Compiler Makefile
# Cross-platform Makefile for Windows and Linux

# Detect OS
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)

# Compiler settings based on OS
ifeq ($(UNAME_S),Linux)
    # Linux settings
    CXX = g++
    CC = gcc
    LEX = flex
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
    EXE_SUFFIX = 
    PATH_SEP = /
else ifeq ($(UNAME_S),Darwin)
    # macOS settings
    CXX = g++
    CC = gcc
    LEX = flex
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
    EXE_SUFFIX = 
    PATH_SEP = /
else
    # Windows settings (MinGW/MSYS/Git Bash)
    CXX = g++
    CC = gcc
    LEX = flex
    RM = del /f
    RMDIR = rmdir /s /q
    MKDIR = mkdir
    EXE_SUFFIX = .exe
    PATH_SEP = \\
endif

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
LDFLAGS = 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
EXAMPLES_DIR = examples
TESTS_DIR = tests
DOCS_DIR = docs

# Main compiler
COMPILER = chetona$(EXE_SUFFIX)
COMPILER_SRC = $(SRC_DIR)/main.cpp

# Flex lexer
LEXER = chetona_lexer$(EXE_SUFFIX)
LEXER_SRC = chetona.l
LEXER_GEN = lex.yy.c

# Test executable
TEST_EXE = test_compiler$(EXE_SUFFIX)
TEST_SRC = $(TESTS_DIR)/test_compiler.cpp

# Output files
OUTPUT_CPP = output.cpp
OUTPUT_EXE = output$(EXE_SUFFIX)

# Colors for output (works on Linux/macOS, ignored on Windows)
ifeq ($(EXE_SUFFIX),)
    RED = \033[0;31m
    GREEN = \033[0;32m
    YELLOW = \033[0;33m
    BLUE = \033[0;34m
    RESET = \033[0m
else
    RED = 
    GREEN = 
    YELLOW = 
    BLUE = 
    RESET = 
endif

.PHONY: all clean compiler lexer test examples help dirs info

# Default target
all: dirs compiler lexer
	@echo "$(GREEN)Build complete!$(RESET)"
	@echo "Run 'make help' for usage information."

# Create build directory
dirs:
	@$(MKDIR) $(BUILD_DIR)

# Build the main compiler
compiler: dirs $(BUILD_DIR)/$(COMPILER)

$(BUILD_DIR)/$(COMPILER): $(COMPILER_SRC)
	@echo "$(BLUE)Compiling Chetona compiler...$(RESET)"
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
	@echo "$(GREEN)Compiler built: $@$(RESET)"

# Build the Flex lexer
lexer: dirs $(BUILD_DIR)/$(LEXER)

$(BUILD_DIR)/$(LEXER): $(LEXER_GEN)
	@echo "$(BLUE)Building Flex lexer...$(RESET)"
	$(CXX) $(LEXER_GEN) -o $@
	@echo "$(GREEN)Lexer built: $@$(RESET)"

$(LEXER_GEN): $(LEXER_SRC)
	@echo "$(BLUE)Generating lexer from Flex source...$(RESET)"
	$(LEX) $(LEXER_SRC)

# Build and run tests
test: dirs $(BUILD_DIR)/$(TEST_EXE)
	@echo "$(BLUE)Running tests...$(RESET)"
	./$(BUILD_DIR)/$(TEST_EXE)

$(BUILD_DIR)/$(TEST_EXE): $(TEST_SRC)
	@echo "$(BLUE)Building test suite...$(RESET)"
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Compile a Chetona file (use FILE=xxx)
compile: compiler
ifndef FILE
	@echo "$(RED)Error: No input file specified.$(RESET)"
	@echo "Usage: make compile FILE=examples/hello.chetona"
else
	@echo "$(BLUE)Compiling $(FILE)...$(RESET)"
	./$(BUILD_DIR)/$(COMPILER) $(FILE) -o $(OUTPUT_CPP) -c
	@echo "$(GREEN)Output: $(OUTPUT_CPP)$(RESET)"
endif

# Run the lexer on a file (use FILE=xxx)
lex: lexer
ifndef FILE
	@echo "$(RED)Error: No input file specified.$(RESET)"
	@echo "Usage: make lex FILE=examples/hello.chetona"
else
	@echo "$(BLUE)Lexing $(FILE)...$(RESET)"
	./$(BUILD_DIR)/$(LEXER) $(FILE)
endif

# Run a specific example
run: compiler
ifndef FILE
	@echo "$(RED)Error: No input file specified.$(RESET)"
	@echo "Usage: make run FILE=examples/hello.chetona"
else
	@echo "$(BLUE)Compiling and running $(FILE)...$(RESET)"
	./$(BUILD_DIR)/$(COMPILER) $(FILE) -o $(OUTPUT_CPP) -t -a -c
	./$(OUTPUT_EXE)
endif

# Build all examples
examples: compiler
	@echo "$(BLUE)Building all examples...$(RESET)"
	@for file in $(EXAMPLES_DIR)/*.chetona; do \
		echo "  Processing $$file..."; \
		./$(BUILD_DIR)/$(COMPILER) $$file -o $(BUILD_DIR)/$$(basename $$file .chetona).cpp; \
	done
	@echo "$(GREEN)All examples built in $(BUILD_DIR)/$(RESET)"

# Run all examples
run-examples: examples
	@echo "$(BLUE)Compiling all examples to executables...$(RESET)"
	@for file in $(BUILD_DIR)/*.cpp; do \
		echo "  Compiling $$file..."; \
		$(CXX) -std=c++11 $$file -o $${file%.cpp}$(EXE_SUFFIX); \
	done
	@echo "$(GREEN)All examples compiled.$(RESET)"

# Show token output for an example
tokens: lexer
	@echo "$(BLUE)Tokenizing hello.chetona...$(RESET)"
	./$(BUILD_DIR)/$(LEXER) $(EXAMPLES_DIR)/hello.chetona

# Clean build files
clean:
	@echo "$(YELLOW)Cleaning build files...$(RESET)"
ifeq ($(EXE_SUFFIX),.exe)
	@if exist $(BUILD_DIR) $(RMDIR) $(BUILD_DIR)
	@if exist $(LEXER_GEN) $(RM) $(LEXER_GEN)
	@if exist $(OUTPUT_CPP) $(RM) $(OUTPUT_CPP)
	@if exist $(OUTPUT_EXE) $(RM) $(OUTPUT_EXE)
else
	$(RMDIR) $(BUILD_DIR)
	$(RM) -f $(LEXER_GEN) $(OUTPUT_CPP) $(OUTPUT_EXE)
endif
	@echo "$(GREEN)Clean complete.$(RESET)"

# Clean everything including generated files
distclean: clean
	@echo "$(YELLOW)Deep cleaning...$(RESET)"
ifeq ($(EXE_SUFFIX),.exe)
	@if exist *.o $(RM) *.o
	@if exist *.exe $(RM) *.exe
else
	$(RM) -f *.o *.exe
endif
	@echo "$(GREEN)Deep clean complete.$(RESET)"

# Rebuild from scratch
rebuild: clean all

# Show system information
info:
	@echo "$(BLUE)=== Chetona Compiler Build Info ===$(RESET)"
	@echo "Operating System: $(UNAME_S)"
	@echo "C++ Compiler: $(CXX)"
	@echo "C Compiler: $(CC)"
	@echo "Lexer: $(LEX)"
	@echo "Executable suffix: $(EXE_SUFFIX)"
	@echo ""
	@echo "Build directories:"
	@echo "  Source: $(SRC_DIR)"
	@echo "  Include: $(INCLUDE_DIR)"
	@echo "  Build: $(BUILD_DIR)"
	@echo "  Examples: $(EXAMPLES_DIR)"
	@echo "  Tests: $(TESTS_DIR)"
	@echo "  Docs: $(DOCS_DIR)"

# Help
help:
	@echo "$(BLUE)Chetona Compiler Build System$(RESET)"
	@echo ""
	@echo "$(GREEN)Build Targets:$(RESET)"
	@echo "  all          - Build compiler and lexer (default)"
	@echo "  compiler     - Build only the main compiler"
	@echo "  lexer        - Build only the Flex lexer"
	@echo "  test         - Build and run unit tests"
	@echo "  examples     - Build all example programs"
	@echo "  run-examples - Compile all examples to executables"
	@echo ""
	@echo "$(GREEN)Usage Targets:$(RESET)"
	@echo "  compile      - Compile a Chetona file (FILE=xxx)"
	@echo "  lex          - Run lexer on a file (FILE=xxx)"
	@echo "  run          - Compile and run a file (FILE=xxx)"
	@echo "  tokens       - Show token output for hello.chetona"
	@echo ""
	@echo "$(GREEN)Maintenance Targets:$(RESET)"
	@echo "  clean        - Remove build artifacts"
	@echo "  distclean    - Deep clean all generated files"
	@echo "  rebuild      - Clean and rebuild everything"
	@echo "  info         - Show build configuration"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "$(YELLOW)Examples:$(RESET)"
	@echo "  make compile FILE=examples/hello.chetona"
	@echo "  make lex FILE=examples/calculator.chetona"
	@echo "  make run FILE=examples/porikkha.chetona"
	@echo "  make test"
	@echo "  make info"
	@echo ""
	@echo "$(YELLOW)Detected OS: $(UNAME_S)$(RESET)"
