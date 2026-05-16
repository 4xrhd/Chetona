#!/bin/bash
# Chetona Compiler Build Script for Linux/macOS

# Configuration
CXX="${CXX:-g++}"
CC="${CC:-gcc}"
LEX="${LEX:-flex}"
BUILD_DIR="build"
SRC_DIR="src"
INCLUDE_DIR="include"
EXAMPLES_DIR="examples"
TESTS_DIR="tests"
CXXFLAGS="-std=c++11 -Wall -Wextra -I./$INCLUDE_DIR"
COMPILER="chetona"
LEXER="chetona_lexer"
LEXER_SRC="chetona.l"
LEXER_GEN="lex.yy.c"
TEST_EXE="test_compiler"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

# Functions
create_dirs() {
    mkdir -p "$BUILD_DIR"
}

build_compiler() {
    echo -e "${BLUE}Compiling Chetona compiler...${RESET}"
    $CXX $CXXFLAGS "$SRC_DIR/main.cpp" -o "$BUILD_DIR/$COMPILER"
    if [ $? -ne 0 ]; then
        echo -e "${RED}Compiler build failed!${RESET}"
        exit 1
    fi
    echo -e "${GREEN}Compiler built: $BUILD_DIR/$COMPILER${RESET}"
}

build_lexer() {
    echo -e "${BLUE}Generating lexer from Flex source...${RESET}"
    $LEX $LEXER_SRC
    if [ $? -ne 0 ]; then
        echo -e "${RED}Flex generation failed!${RESET}"
        exit 1
    fi
    echo -e "${BLUE}Building Flex lexer...${RESET}"
    $CXX $LEXER_GEN -o "$BUILD_DIR/$LEXER"
    if [ $? -ne 0 ]; then
        echo -e "${RED}Lexer build failed!${RESET}"
        exit 1
    fi
    echo -e "${GREEN}Lexer built: $BUILD_DIR/$LEXER${RESET}"
}

build_all() {
    create_dirs
    build_compiler
    build_lexer
    echo -e "${GREEN}Build complete!${RESET}"
}

build_test() {
    create_dirs
    echo -e "${BLUE}Building test suite...${RESET}"
    $CXX $CXXFLAGS "$TESTS_DIR/test_compiler.cpp" -o "$BUILD_DIR/$TEST_EXE"
    if [ $? -ne 0 ]; then
        echo -e "${RED}Test build failed!${RESET}"
        exit 1
    fi
    echo -e "${BLUE}Running tests...${RESET}"
    "./$BUILD_DIR/$TEST_EXE"
}

build_examples() {
    create_dirs
    if [ ! -f "$BUILD_DIR/$COMPILER" ]; then
        build_compiler
    fi
    echo -e "${BLUE}Building all examples...${RESET}"
    for file in "$EXAMPLES_DIR"/*.chetona; do
        if [ -f "$file" ]; then
            echo "  Processing $file..."
            basename="${file##*/}"
            name="${basename%.chetona}"
            "./$BUILD_DIR/$COMPILER" "$file" -o "$BUILD_DIR/$name.cpp"
        fi
    done
    echo -e "${GREEN}All examples built in $BUILD_DIR/${RESET}"
}

compile_file() {
    if [ -z "$1" ]; then
        echo -e "${RED}Error: No input file specified.${RESET}"
        echo "Usage: $0 compile <file.chetona>"
        exit 1
    fi
    if [ ! -f "$BUILD_DIR/$COMPILER" ]; then
        build_compiler
    fi
    echo -e "${BLUE}Compiling $1...${RESET}"
    "./$BUILD_DIR/$COMPILER" "$1" -o output.cpp -c
    echo -e "${GREEN}Output: output.cpp${RESET}"
}

lex_file() {
    if [ -z "$1" ]; then
        echo -e "${RED}Error: No input file specified.${RESET}"
        echo "Usage: $0 lex <file.chetona>"
        exit 1
    fi
    if [ ! -f "$BUILD_DIR/$LEXER" ]; then
        build_lexer
    fi
    echo -e "${BLUE}Lexing $1...${RESET}"
    "./$BUILD_DIR/$LEXER" "$1"
}

run_file() {
    if [ -z "$1" ]; then
        echo -e "${RED}Error: No input file specified.${RESET}"
        echo "Usage: $0 run <file.chetona>"
        exit 1
    fi
    if [ ! -f "$BUILD_DIR/$COMPILER" ]; then
        build_compiler
    fi
    echo -e "${BLUE}Compiling and running $1...${RESET}"
    "./$BUILD_DIR/$COMPILER" "$1" -o output.cpp -t -a -c
    ./output
}

clean() {
    echo -e "${YELLOW}Cleaning build files...${RESET}"
    rm -rf "$BUILD_DIR"
    rm -f "$LEXER_GEN" output.cpp output
    echo -e "${GREEN}Clean complete.${RESET}"
}

distclean() {
    clean
    echo -e "${YELLOW}Deep cleaning...${RESET}"
    rm -f *.o *.exe
    echo -e "${GREEN}Deep clean complete.${RESET}"
}

show_info() {
    echo -e "${BLUE}=== Chetona Compiler Build Info ===${RESET}"
    echo "Operating System: $(uname -s)"
    echo "C++ Compiler: $CXX"
    echo "C Compiler: $CC"
    echo "Lexer: $LEX"
    echo ""
    echo "Build directories:"
    echo "  Source: $SRC_DIR"
    echo "  Include: $INCLUDE_DIR"
    echo "  Build: $BUILD_DIR"
    echo "  Examples: $EXAMPLES_DIR"
    echo "  Tests: $TESTS_DIR"
}

show_help() {
    echo -e "${BLUE}Chetona Compiler Build System for Linux/macOS${RESET}"
    echo ""
    echo -e "${GREEN}Build Targets:${RESET}"
    echo "  all          - Build compiler and lexer (default)"
    echo "  compiler     - Build only the main compiler"
    echo "  lexer        - Build only the Flex lexer"
    echo "  test         - Build and run unit tests"
    echo "  examples     - Build all example programs"
    echo ""
    echo -e "${GREEN}Usage Targets:${RESET}"
    echo "  compile FILE - Compile a Chetona file"
    echo "  lex FILE     - Run lexer on a file"
    echo "  run FILE     - Compile and run a file"
    echo ""
    echo -e "${GREEN}Maintenance Targets:${RESET}"
    echo "  clean        - Remove build artifacts"
    echo "  distclean    - Deep clean all generated files"
    echo "  rebuild      - Clean and rebuild everything"
    echo "  info         - Show build configuration"
    echo "  help         - Show this help message"
    echo ""
    echo -e "${YELLOW}Examples:${RESET}"
    echo "  $0 all"
    echo "  $0 compile examples/hello.chetona"
    echo "  $0 lex examples/calculator.chetona"
    echo "  $0 run examples/porikkha.chetona"
    echo "  $0 test"
    echo "  $0 clean"
}

# Main
case "${1:-}" in
    all|"")
        build_all
        ;;
    compiler)
        create_dirs
        build_compiler
        ;;
    lexer)
        create_dirs
        build_lexer
        ;;
    test)
        build_test
        ;;
    examples)
        build_examples
        ;;
    compile)
        compile_file "$2"
        ;;
    lex)
        lex_file "$2"
        ;;
    run)
        run_file "$2"
        ;;
    clean)
        clean
        ;;
    distclean)
        distclean
        ;;
    rebuild)
        clean
        build_all
        ;;
    info)
        show_info
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        echo -e "${RED}Unknown target: $1${RESET}"
        show_help
        exit 1
        ;;
esac
