#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel

# Verifica se os argumentos foram passados corretamente
if [ "$#" -ne 2 ]; then
    echo "Usage: extract_extern <extern_name> <file>"
    exit 1
fi

EXTERN_NAME="$1"
FILE="$2"

# Verifica se o arquivo existe
if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' not found."
    exit 1
fi

# Tenta extrair as declarações extern usando clang
EXTERN_OUTPUT=$(clang -E -fsyntax-only "$FILE" 2>/dev/null | grep "extern")

# Verifica se a declaração aparece no retorno do clang
RESULT=$(echo "$EXTERN_OUTPUT" | grep -E "extern[[:space:]]+.*$EXTERN_NAME[[:space:]]*\(")
if [ -n "$RESULT" ]; then
    echo "$RESULT"
    exit 0
fi

# Se clang falhar completamente ou não encontrar a declaração, tenta com gcc
if [ $? -ne 0 ] || [ -z "$RESULT" ]; then
    EXTERN_OUTPUT=$(gcc -E -fsyntax-only "$FILE" 2>/dev/null | grep "extern")
    RESULT=$(echo "$EXTERN_OUTPUT" | grep -E "extern[[:space:]]+.*$EXTERN_NAME[[:space:]]*\(")
    if [ -n "$RESULT" ]; then
        echo "$RESULT"
        exit 0
    fi
fi

# Se nenhum dos compiladores encontrar a declaração, retorna erro
echo "Error: Extern '$EXTERN_NAME' not found in '$FILE'."
exit 1
