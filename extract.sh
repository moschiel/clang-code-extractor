#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel


SEARCH_TYPES="function, prototype, typedef, struct, union, enum, macro, global, extern-variable, extern-function"

# Verifica se os argumentos foram passados corretamente
if [ "$#" -lt 3 ]; then
    echo "Usage: $0 <type> <name> <source_file> [extra_args]"
    echo "Types available: $SEARCH_TYPES"
    exit 1
fi

# Parâmetros
EXTRACT_TYPE="$1"  # Tipo do que será extraído (function, typedef, etc.)
NAME="$2"          # Nome da entidade a extrair
SOURCE_FILE="$3"   # Caminho do arquivo fonte

# Verifica se o arquivo existe
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found."
    exit 1
fi

# Define a flag de extração baseada no tipo fornecido
case "$EXTRACT_TYPE" in
    function)           GREP_TYPE="Function"        ;;
    macro)              GREP_TYPE="Macro"           ;;
    global)             GREP_TYPE="Global Variable" ;;
    extern-function)    GREP_TYPE="Extern Function" ;;
    extern-variable)    GREP_TYPE="Extern Variable" ;;
    prototype)          GREP_TYPE="Prototype"       ;;
    typedef)            GREP_TYPE="Typedef"         ;;
    struct)             GREP_TYPE="Struct"          ;;
    union)              GREP_TYPE="Union"           ;;
    enum)               GREP_TYPE="Enum"            ;;
    *)
        echo "Error: Unknown type '$EXTRACT_TYPE'."
        echo "Available types: $SEARCH_TYPES"
        exit 1
        ;;
esac


# Se um quarto argumento for fornecido, use-o como extra-args
EXTRA_ARGS=""
RETURN_LINES=false
if [ "$#" -ge 4 ]; then
    #EXTRA_ARGS=$4
    #EXTRA_ARGS="-extra-arg=$4"
    #echo "EXTRA_ARGS=$4"
    # Usa o IFS para separar em um array
    IFS=' ' read -ra ARGS <<< "$4"
    # Itera sobre cada elemento do array e imprime
    for arg in "${ARGS[@]}"; do
        if [ $arg == "lines" ]; then
            #Se for lines, eh pra retornar apenas as linhas em que esta localizado
            RETURN_LINES=true
            continue
        fi
        #echo "Argumento: $arg"
        EXTRA_ARGS+="-extra-arg=$arg "
    done
fi

# Executa o extractor e armazena a saída
#echo "./build/extractor "$SOURCE_FILE" -"$EXTRACT_TYPE"="$NAME" "$EXTRA_ARGS""
EXTRACTOR_OUTPUT=$(./build/extractor "$SOURCE_FILE" -"$EXTRACT_TYPE"="$NAME" $EXTRA_ARGS 2>&1)

# Encontra a linha onde está o padrão "<TYPE>: <NAME>"
line_number=$(echo "$EXTRACTOR_OUTPUT" | grep -n -E "^($GREP_TYPE): $NAME" | cut -d: -f1)

# Capturar as duas linhas abaixo do padrão
start_line_raw=$(echo "$EXTRACTOR_OUTPUT" | sed -n "$((line_number+1))p")
end_line_raw=$(echo "$EXTRACTOR_OUTPUT" | sed -n "$((line_number+2))p")

# Verifica se o elemento foi encontrado
if [[ -n "$line_number" ]]; then

    if $RETURN_LINES && [[ "$start_line_raw" =~ ^Start\ Line: ]] && [[ "$end_line_raw" =~ ^End\ Line: ]]; then
        # Capturar os numeros das linhas
        start_line=$(echo "$start_line_raw" | awk -F': ' '{print $2}')
        end_line=$(echo "$end_line_raw" | awk -F': ' '{print $2}')

        # Verificar se Start Line é maior que End Line
        if (( start_line > end_line )); then
            echo "Erro: '$NAME' Start Line ($start_line) é maior que End Line ($end_line)."
            exit 1
        fi

        # Concatenar os valores no formato "<um valor>;<outro valor>"
        EXTRACTED_LINES="${start_line};${end_line}"
        
        echo "$EXTRACTED_LINES"
    else
        # Extrai o código do elemento após "Definition:"
        EXTRACTED_BODY=$(echo "$EXTRACTOR_OUTPUT" | awk '/Definition:/,0' | sed '1d')

        # Exibe a definição extraída
        echo "$EXTRACTED_BODY"
    fi
else
    echo "Error: '$NAME' not found in '$SOURCE_FILE'."
    exit 1
fi