#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel
#
# extract_global.sh
#
# Extrai a declaração textual de uma variável global (mesmo com macros indefinidas)
# usando o clang -ast-dump para localizar a linha de início e, em seguida, lendo
# o arquivo fonte original (que mantém a numeração de linhas) até o primeiro ponto-e-vírgula.
#
# Uso: extract_global.sh <global_variable_name> <file>

if [ "$#" -ne 2 ]; then
    echo "Usage: extract_global.sh <global_variable_name> <file>"
    exit 1
fi

GLOBAL_VAR_NAME="$1"
FILE="$2"

# Verifica se o arquivo existe
if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' not found."
    exit 1
fi

# Usa clang -ast-dump para obter o AST e filtrar a linha que contém a declaração exata da variável
AST_OUTPUT=$(clang -Xclang -ast-dump -fsyntax-only "$FILE" 2>&1 | grep -E "VarDecl" | grep -w "$GLOBAL_VAR_NAME")
if [ -n "$AST_OUTPUT" ]; then
    # Captura o número da linha, aceitando tanto <line:... quanto <nome_arquivo:...
    # Usamos 'head -n1' para pegar somente o primeiro match.
    LINE=$(echo "$AST_OUTPUT" | grep -oP '<(?:line|[^:]+):\K[0-9]+' | head -n1)
    if [ -z "$LINE" ]; then
        echo "Error: Could not determine the line number for variable '$GLOBAL_VAR_NAME'."
        exit 1
    fi
else
    echo "Error: Global variable '$GLOBAL_VAR_NAME' not found."
    exit 1
fi

# Agora, usamos o arquivo original para extrair o bloco textual,
# a partir da linha identificada até encontrar o primeiro ponto-e-vírgula.
EXTRACTED=$(sed -n "${LINE},\$p" "$FILE" | sed '/;/q')

if [ -z "$EXTRACTED" ]; then
    echo "Error: Could not extract global variable definition for '$GLOBAL_VAR_NAME'."
    exit 1
fi

echo "$EXTRACTED"
exit 0
