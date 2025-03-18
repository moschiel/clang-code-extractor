#!/usr/bin/env python3
import os
import subprocess
import sys

def main():
    build_dir = 'build'
    
    # Cria o diretório "build" se não existir
    if not os.path.isdir(build_dir):
        os.mkdir(build_dir)
    
    # Entra no diretório "build"
    os.chdir(build_dir)
    
    # Executa o cmake para gerar os arquivos de build
    ret = subprocess.run(['cmake', '..'])
    if ret.returncode != 0:
        sys.exit(ret.returncode)
    
    # Compila o projeto de forma cross-platform
    ret = subprocess.run(['cmake', '--build', '.'])
    sys.exit(ret.returncode)

if __name__ == '__main__':
    main()
