#!/usr/bin/env python3
import os
import sys
import subprocess
import platform
import shutil

def main():
    build_dir = 'build'
    # Se desejar um build limpo, descomente a linha abaixo:
    # if os.path.isdir(build_dir): os.system("rm -rf " + build_dir)
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)
    os.mkdir(build_dir)
    os.chdir(build_dir)
    system = platform.system()

    env = os.environ.copy()
    if system == "Windows":
         # Tenta encontrar Cmake no PATH
        cmake_path = shutil.which("cmake")
        if not cmake_path:
            # No windows, tem que passar o diretorio completo do Cmake, adiciona ao PATH nao funciona nao sei porque
            cmake_path = "C:\\Program Files\\CMake\\bin\\cmake.exe" 
            # Verifica se o cmake existe
            if not os.path.exists(cmake_path):
                raise FileNotFoundError("CMake não encontrado!")

        generator = "MinGW Makefiles"
        print("Using generator:", generator)
        cmake_cmd = [
            cmake_path,
            "-G", generator,
            "..",
            #"-DCMAKE_BUILD_TYPE=Release",
            #"-DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe",
            #"-DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe",
            #"-DCMAKE_MAKE_PROGRAM=C:/msys64/mingw64/bin/mingw32-make.exe"
        ]
        ret = subprocess.run(cmake_cmd, env=env)
    else:
        cmake_path = "cmake" # No ubuntu, funciona de qualquer diretorio
        ret = subprocess.run([cmake_path, ".."], env=env)
    if ret.returncode != 0:
        sys.exit(ret.returncode)
    
    ret = subprocess.run([cmake_path, "--build", "."], env=env)
    sys.exit(ret.returncode)

if __name__ == '__main__':
    main()
