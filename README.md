# clang-code-extractor

Este projeto utiliza LLVM/Clang para extrair definições de código C/C++ (por exemplo, funções, typedefs, estruturas, etc.). A seguir estão as instruções para configurar o ambiente e compilar o projeto em **Linux** e em **Windows** (nativamente).

---

## 1. Linux (Ubuntu)

### Dependências

Em distribuições baseadas no Ubuntu, por exemplo, execute:

```bash
sudo apt update
sudo apt install build-essential clang llvm clang-tools-extra cmake libxml2-dev
```

- **build-essential:** Inclui gcc, g++, make e outras ferramentas básicas de compilação.
- **clang & llvm:** Fornecem o compilador Clang e as ferramentas LLVM.
- **clang-tools-extra:** Ferramentas extras para o Clang.
- **cmake:** Para gerar os arquivos de build.
- **libxml2-dev:** Dependência para algumas funcionalidades.

### Build

1. **Clone o repositório (incluindo submódulos):**

   ```bash
   git clone https://github.com/moschiel/clang-code-extractor.git
   cd clang-code-extractor
   ```

2. **Compile o projeto:**

   ```bash
   python3 build.py
   ```
   O executável será gerado em  build/extractor

3. **Execute a funcionalidade com o script extract.py:**

   ```bash
   python3 extract.py <type> <name> <source_file> [extra_args]
   ```

---

## 2. Windows (usando MSYS2)

### Dependências

1. **Instale o MSYS2:**  
   Baixe e instale o [MSYS2](https://www.msys2.org/).

2. **Atualize o MSYS2:**

   Abra o terminal **MSYS2 MinGW 64-bit** e execute:

   ```bash
   pacman -Syu
   ```

3. **Instale as ferramentas necessárias:**

   No terminal MSYS2, execute:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-clang mingw-w64-x86_64-clang-tools-extra mingw-w64-x86_64-llvm mingw-w64-x86_64-make
   ```
   - **mingw-w64-x86_64-gcc:** Compilador GCC do MinGW.
   - **mingw-w64-x86_64-clang:** Compilador Clang para Windows.
   - **mingw-w64-x86_64-clang-tools-extra:** Ferramentas extras para o Clang.
   - **mingw-w64-x86_64-llvm:** LLVM e arquivos de configuração (por exemplo, `LLVMConfig.cmake`).
   - **mingw-w64-x86_64-make:** Fornece o `mingw32-make.exe`.

4. **Verifique o PATH:**

   Certifique-se de que o diretório `C:\msys64\mingw64\bin` esteja no PATH do sistema, para que os executáveis (como `clang.exe`, `gcc.exe`, etc.) possam ser encontrados.

### Build

1. **Clone o repositório (incluindo submódulos):**

   ```bash
   git clone https://github.com/moschiel/clang-code-extractor.git
   cd clang-code-extractor
   ```

2. **Compile o projeto:**

   ```bash
   python build.py
   ```
   O executável será gerado em  build/extractor.exe

3. **Execute a funcionalidade com o script extract.py:**

   ```bash
   python extract.py <type> <name> <source_file> [extra_args]
   ```

---


> **Observação:**  
> No Windows, os caminhos para `LLVM_DIR` e `Clang_DIR` devem corresponder à sua instalação MSYS2. Se os caminhos forem diferentes, atualize as configurações do arquivo 'windows-config.cmake' .
---


