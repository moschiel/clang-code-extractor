cmake_minimum_required(VERSION 3.10)
project(SourceExtractor)

# Procura pelo LLVM e Clang (usando os pacotes CONFIG)
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Inclui os diretórios de include do LLVM e Clang
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Lista dos arquivos fonte (no nosso caso, extractor.cpp)
set(SOURCE_FILES extractor.cpp)

# Cria o executável
add_executable(extractor ${SOURCE_FILES})

# Linka com as bibliotecas necessárias
target_link_libraries(extractor
    clangTooling
    clangFrontendTool
    clangFrontend
    clangDriver
    clangSerialization
    clangASTMatchers
    clangAST
    clangBasic
    clangLex
    clangEdit
    clangSema
    LLVM
)
