#!/usr/bin/env python3
import os
import sys
import subprocess
import re

def is_error_line(line):
    """Retorna True se a linha aparentar ser uma mensagem de erro."""
    trimmed = line.strip()
    error_patterns = [
        r'^Error while',                      # Ex.: "Error while trying to load ..."
        r'^No compilation database',          # Ex.: "No compilation database found ..."
        r'^fixed-compilation-database:',      # Ex.: "fixed-compilation-database: Error while opening ..."
        r'^json-compilation-database:',       # Ex.: "json-compilation-database: Error while opening ..."
        r'^\S.*error:'                        # Linhas que começam com algum texto e contém "error:"
    ]
    for pattern in error_patterns:
        if re.search(pattern, trimmed):
            return True
    return False

def extract_definition_until_error(lines):
    """
    Extrai a definição acumulando linhas até encontrar uma mensagem de erro.
    """
    definition_lines = []
    for line in lines:
        if is_error_line(line):
            break
        definition_lines.append(line)
    return definition_lines

def extract_line_numbers(lines):
    """
    Procura nas linhas por "Start Line:" e "End Line:" e retorna (start, end)
    Se não encontrar, retorna (None, None)
    """
    start_line = None
    end_line = None
    for line in lines:
        if line.startswith("Start Line:"):
            try:
                start_line = int(line.split(": ", 1)[1])
            except (IndexError, ValueError):
                pass
        elif line.startswith("End Line:"):
            try:
                end_line = int(line.split(": ", 1)[1])
            except (IndexError, ValueError):
                pass
        if start_line is not None and end_line is not None:
            break
    return start_line, end_line

def main():
    # Diretório do script (para localizar o executável relativo)
    extract_script_dir = os.path.dirname(os.path.abspath(__file__))
    search_types = "function, prototype, typedef, struct, union, enum, macro, global, extern-variable, extern-function"

    if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} <type> <name> <source_file> [extra_args]")
        print(f"Types available: {search_types}")
        sys.exit(1)

    # Parâmetros
    extract_type = sys.argv[1]  # Tipo a extrair (function, typedef, etc.)
    name = sys.argv[2]          # Nome da entidade
    source_file = sys.argv[3]   # Arquivo fonte

    if not os.path.isfile(source_file):
        print(f"Error: Source file '{source_file}' not found.")
        sys.exit(1)

    # Mapeia o tipo para a flag esperada (GREP_TYPE)
    type_map = {
        "function": "Function",
        "macro": "Macro",
        "global": "Global Variable",
        "extern-function": "Extern Function",
        "extern-variable": "Extern Variable",
        "prototype": "Prototype",
        "typedef": "Typedef",
        "struct": "Struct",
        "union": "Union",
        "enum": "Enum",
    }
    grep_type = type_map.get(extract_type)
    if grep_type is None:
        print(f"Error: Unknown type '{extract_type}'.")
        print(f"Available types: {search_types}")
        sys.exit(1)

    # Processa argumentos extras (se houver)
    extra_args = []
    return_lines = False
    if len(sys.argv) >= 5:
        for arg in sys.argv[4:]:
            filtered_arg = arg.replace('\r', '').replace('\n', '')
            if filtered_arg == "lines":
                return_lines = True
                continue
            extra_args.append(f"-extra-arg={filtered_arg}")

    # Monta o comando para chamar o executável "extractor"
    extractor_executable = os.path.join(extract_script_dir, "build", "extractor")
    command = [extractor_executable, source_file, f"-{extract_type}={name}"] + extra_args

    try:
        result = subprocess.run(command, capture_output=True, text=True)
    except Exception as e:
        print(f"Error executing extractor: {e}")
        sys.exit(1)

    # Concatena stdout e stderr (como 2>&1 no shell)
    extractor_output = result.stdout + result.stderr
    all_lines = extractor_output.splitlines()

    # Se o modo "lines" foi solicitado, extrai e imprime as informações de linha
    if return_lines:
        s_line, e_line = extract_line_numbers(all_lines)
        if s_line is None or e_line is None:
            print("Error: Start Line or End Line information not found.")
            sys.exit(1)
        if s_line > e_line:
            print(f"Erro: '{name}' Start Line ({s_line}) é maior que End Line ({e_line}).")
            sys.exit(1)
        print(f"{s_line};{e_line}")
        sys.exit(0)

    # Tenta extrair start/end lines para determinar a quantidade de linhas da definição
    s_line, e_line = extract_line_numbers(all_lines)
    if s_line is not None and e_line is not None:
        total_def_lines = e_line - s_line + 1
        # Procura a linha "Definition:" no output
        definition_index = None
        for i, line in enumerate(all_lines):
            if "Definition:" in line:
                definition_index = i
                break
        if definition_index is not None:
            # Extrai exatamente o número de linhas esperado após "Definition:"
            extracted_block = all_lines[definition_index+1 : definition_index+1+total_def_lines]
            if extracted_block:
                print("\n".join(extracted_block))
                sys.exit(0)
    # Se não encontrou os marcadores ou se o bloco extraído ficou vazio, faz extração padrão:
    extracted_block = extract_definition_until_error(all_lines)
    if extracted_block:
        print("\n".join(extracted_block))
    else:
        print(f"Error: Definition for '{name}' not found in '{source_file}'.")
        sys.exit(1)

if __name__ == "__main__":
    main()
