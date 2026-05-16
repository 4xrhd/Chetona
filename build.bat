@echo off
REM Chetona Compiler Build Script for Windows
REM Requires MinGW or similar GCC setup

setlocal enabledelayedexpansion

REM Configuration
set CXX=g++
set CC=gcc
set LEX=flex
set BUILD_DIR=build
set SRC_DIR=src
set INCLUDE_DIR=include
set EXAMPLES_DIR=examples
set TESTS_DIR=tests
set CXXFLAGS=-std=c++11 -Wall -Wextra -I./%INCLUDE_DIR%
set COMPILER=chetona.exe
set LEXER=chetona_lexer.exe
set LEXER_SRC=chetona.l
set LEXER_GEN=lex.yy.c
set TEST_EXE=test_compiler.exe

REM Colors (using ANSI escape codes - Windows 10+)
set "RED=[91m"
set "GREEN=[92m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "RESET=[0m"

:main
if "%1"=="" goto help
if "%1"=="all" goto all
if "%1"=="compiler" goto compiler
if "%1"=="lexer" goto lexer
if "%1"=="test" goto test
if "%1"=="examples" goto examples
if "%1"=="clean" goto clean
if "%1"=="distclean" goto distclean
if "%1"=="rebuild" goto rebuild
if "%1"=="info" goto info
if "%1"=="help" goto help
if "%1"=="compile" goto compile
if "%1"=="lex" goto lex_run
if "%1"=="run" goto run
goto help

:all
call :create_dirs
call :build_compiler
call :build_lexer
echo %GREEN%Build complete!%RESET%
goto end

:compiler
call :create_dirs
call :build_compiler
goto end

:lexer
call :create_dirs
call :build_lexer
goto end

:test
call :create_dirs
echo %BLUE%Building test suite...%RESET%
%CXX% %CXXFLAGS% %TESTS_DIR%\test_compiler.cpp -o %BUILD_DIR%\%TEST_EXE%
if errorlevel 1 (
    echo %RED%Test build failed!%RESET%
    goto end
)
echo %BLUE%Running tests...%RESET%
%BUILD_DIR%\%TEST_EXE%
goto end

:examples
call :create_dirs
if not exist %BUILD_DIR%\%COMPILER% (
    call :build_compiler
)
echo %BLUE%Building all examples...%RESET%
for %%f in (%EXAMPLES_DIR%\*.chetona) do (
    echo   Processing %%f...
    %BUILD_DIR%\%COMPILER% %%f -o %BUILD_DIR%\%%~nf.cpp
)
echo %GREEN%All examples built in %BUILD_DIR%/%RESET%
goto end

:compile
if "%2"=="" (
    echo %RED%Error: No input file specified.%RESET%
    echo Usage: build.bat compile examples\hello.chetona
    goto end
)
if not exist %BUILD_DIR%\%COMPILER% (
    call :build_compiler
)
echo %BLUE%Compiling %2...%RESET%
%BUILD_DIR%\%COMPILER% %2 -o output.cpp -c
echo %GREEN%Output: output.cpp%RESET%
goto end

:lex_run
if "%2"=="" (
    echo %RED%Error: No input file specified.%RESET%
    echo Usage: build.bat lex examples\hello.chetona
    goto end
)
if not exist %BUILD_DIR%\%LEXER% (
    call :build_lexer
)
echo %BLUE%Lexing %2...%RESET%
%BUILD_DIR%\%LEXER% %2
goto end

:run
if "%2"=="" (
    echo %RED%Error: No input file specified.%RESET%
    echo Usage: build.bat run examples\hello.chetona
    goto end
)
if not exist %BUILD_DIR%\%COMPILER% (
    call :build_compiler
)
echo %BLUE%Compiling and running %2...%RESET%
%BUILD_DIR%\%COMPILER% %2 -o output.cpp -t -a -c
output.exe
goto end

:clean
echo %YELLOW%Cleaning build files...%RESET%
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
if exist %LEXER_GEN% del /f %LEXER_GEN%
if exist output.cpp del /f output.cpp
if exist output.exe del /f output.exe
echo %GREEN%Clean complete.%RESET%
goto end

:distclean
call :clean
echo %YELLOW%Deep cleaning...%RESET%
del /f *.o 2>nul
del /f *.exe 2>nul
echo %GREEN%Deep clean complete.%RESET%
goto end

:rebuild
call :clean
call :all
goto end

:info
echo %BLUE%=== Chetona Compiler Build Info ===%RESET%
echo Operating System: Windows
echo C++ Compiler: %CXX%
echo C Compiler: %CC%
echo Lexer: %LEX%
echo Executable suffix: .exe
echo.
echo Build directories:
echo   Source: %SRC_DIR%
echo   Include: %INCLUDE_DIR%
echo   Build: %BUILD_DIR%
echo   Examples: %EXAMPLES_DIR%
echo   Tests: %TESTS_DIR%
goto end

:help
echo %BLUE%Chetona Compiler Build System for Windows%RESET%
echo.
echo %GREEN%Build Targets:%RESET%
echo   all          - Build compiler and lexer (default)
echo   compiler     - Build only the main compiler
echo   lexer        - Build only the Flex lexer
echo   test         - Build and run unit tests
echo   examples     - Build all example programs
echo.
echo %GREEN%Usage Targets:%RESET%
echo   compile FILE - Compile a Chetona file
echo   lex FILE     - Run lexer on a file
echo   run FILE     - Compile and run a file
echo.
echo %GREEN%Maintenance Targets:%RESET%
echo   clean        - Remove build artifacts
echo   distclean    - Deep clean all generated files
echo   rebuild      - Clean and rebuild everything
echo   info         - Show build configuration
echo   help         - Show this help message
echo.
echo %YELLOW%Examples:%RESET%
echo   build.bat all
echo   build.bat compile examples\hello.chetona
echo   build.bat lex examples\calculator.chetona
echo   build.bat run examples\porikkha.chetona
echo   build.bat test
echo   build.bat clean
goto end

REM Subroutines
:create_dirs
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
goto :eof

:build_compiler
echo %BLUE%Compiling Chetona compiler...%RESET%
%CXX% %CXXFLAGS% %SRC_DIR%\main.cpp -o %BUILD_DIR%\%COMPILER%
if errorlevel 1 (
    echo %RED%Compiler build failed!%RESET%
    exit /b 1
)
echo %GREEN%Compiler built: %BUILD_DIR%\%COMPILER%%RESET%
goto :eof

:build_lexer
echo %BLUE%Generating lexer from Flex source...%RESET%
%LEX% %LEXER_SRC%
if errorlevel 1 (
    echo %RED%Flex generation failed!%RESET%
    exit /b 1
)
echo %BLUE%Building Flex lexer...%RESET%
%CXX% %LEXER_GEN% -o %BUILD_DIR%\%LEXER%
if errorlevel 1 (
    echo %RED%Lexer build failed!%RESET%
    exit /b 1
)
echo %GREEN%Lexer built: %BUILD_DIR%\%LEXER%%RESET%
goto :eof

:end
endlocal
