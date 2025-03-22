#!/usr/bin/env python3
import os
import sys
import subprocess
import platform

def main():
    build_dir = 'build'
    # Se desejar um build limpo, descomente a linha abaixo:
    # if os.path.isdir(build_dir): os.system("rm -rf " + build_dir)
    if not os.path.isdir(build_dir):
        os.mkdir(build_dir)
    os.chdir(build_dir)
    system = platform.system()

    env = os.environ.copy()
    if system == "Windows":
        generator = "MinGW Makefiles"
        print("Using generator:", generator)
        cmake_cmd = [
            "cmake",
            "-G", generator,
            "..",
            #"-DCMAKE_BUILD_TYPE=Release",
            #"-DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe",
            #"-DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe",
            #"-DCMAKE_MAKE_PROGRAM=C:/msys64/mingw64/bin/mingw32-make.exe"
        ]
        ret = subprocess.run(cmake_cmd, env=env)
    else:
        ret = subprocess.run(["cmake", ".."], env=env)
    if ret.returncode != 0:
        sys.exit(ret.returncode)
    
    ret = subprocess.run(["cmake", "--build", "."], env=env)
    sys.exit(ret.returncode)

if __name__ == '__main__':
    main()
