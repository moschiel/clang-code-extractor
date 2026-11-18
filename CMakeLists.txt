cmake_minimum_required(VERSION 3.10)
project(SourceExtractor)

if(WIN32)
  message(STATUS "Configuring for Windows")
  include(${CMAKE_CURRENT_SOURCE_DIR}/windows-config.cmake)
else()
  message(STATUS "Configuring for Linux")
  # No Linux, o CMake usará os caminhos padrão ou aqueles definidos no sistema
endif()

# Procura pelo LLVM e Clang (usando os pacotes CONFIG)
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Inclui os diretórios de include do LLVM e Clang
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
# LLVM packages can export these flags as a space-delimited command string.
# Split explicitly so CMake does not quote all flags as one macro value.
string(REPLACE ";" " " LLVM_DEFINITIONS_COMMAND "${LLVM_DEFINITIONS}")
separate_arguments(LLVM_DEFINITION_FLAGS NATIVE_COMMAND "${LLVM_DEFINITIONS_COMMAND}")
add_definitions(${LLVM_DEFINITION_FLAGS})

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
