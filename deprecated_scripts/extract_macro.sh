#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel

# Verifica se os argumentos foram passados corretamente
if [ "$#" -ne 2 ]; then
    echo "Usage: extract_macro <macro_name> <file>"
    exit 1
fi

MACRO_NAME="$1"
FILE="$2"

# Verifica se o arquivo existe
if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' not found."
    exit 1
fi

# Tenta extrair as macros usando clang
MACRO_OUTPUT=$(clang -E -dM -undef "$FILE" 2>/dev/null)

# Verifica se a macro aparece no retorno do clang
RESULT=$(echo "$MACRO_OUTPUT" | grep -E "^#define[[:space:]]+$MACRO_NAME(\(|[[:space:]])")
if [ -n "$RESULT" ]; then
    echo "$RESULT"
    exit 0
fi

# Se clang falhar completamente ou não encontrar a macro, tenta com gcc
if [ $? -ne 0 ] || [ -z "$RESULT" ]; then
    MACRO_OUTPUT=$(gcc -E -dM -undef "$FILE" 2>/dev/null)
    RESULT=$(echo "$MACRO_OUTPUT" | grep -E "^#define[[:space:]]+$MACRO_NAME(\(|[[:space:]])")
    if [ -n "$RESULT" ]; then
        echo "$RESULT"
        exit 0
    fi
fi

# Se nenhum dos compiladores encontrar a macro, retorna erro
echo "Error: Macro '$MACRO_NAME' not found in '$FILE'."
exit 1