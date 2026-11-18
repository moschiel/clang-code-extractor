# Windows Settings

# Defina os diretórios de configuração do LLVM e Clang conforme sua instalação no MSYS2
set(LLVM_DIR "C:/msys64/mingw64/lib/cmake/llvm" CACHE PATH "LLVM CMake directory")
set(Clang_DIR "C:/msys64/mingw64/lib/cmake/clang" CACHE PATH "Clang CMake directory")

# Use bibliotecas estáticas do LLVM (se necessário)
set(LLVM_USE_STATIC_LIBS ON)

# Força os compiladores e o make program.
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" CACHE STRING "C Compiler")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" CACHE STRING "C++ Compiler")
set(CMAKE_MAKE_PROGRAM "C:/msys64/mingw64/bin/mingw32-make.exe" CACHE STRING "Make Program")
