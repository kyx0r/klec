#!/bin/sh

INPUT_FILE=$1
OUTPUT_DIR=$2
CHUNK_SIZE=$3

if [ -z "$INPUT_FILE" ]; then
  echo "Please specify the input file"
  exit 1
fi

if [ -z "$OUTPUT_DIR" ]; then
  echo "Please specify the output directory"
  exit 1
fi

if [ -z "$CHUNK_SIZE" ]; then
  echo "Please specify the chunk size"
  exit 1
fi

if [ -z "${INPUT_FILE##*.}" ]; then
  FILE_EXTENSION="txt"
else
  FILE_EXTENSION="${INPUT_FILE##*.}"
fi

if [ ! -d "$OUTPUT_DIR" ]; then
  mkdir -p "$OUTPUT_DIR"
fi

COUNT=0
CURRENT_CHUNK=0
OUTPUT_FILE="0-${CHUNK_SIZE}.${FILE_EXTENSION}"
OUTPUT_FILE_PATH="$OUTPUT_DIR/$OUTPUT_FILE"

while IFS= read -r line; do
  ((COUNT++))
  if (( COUNT > CHUNK_SIZE )); then
    ((CURRENT_CHUNK++))
    OUTPUT_FILE="`printf "%d-%d.${FILE_EXTENSION}" $((CURRENT_CHUNK * CHUNK_SIZE)) $((CURRENT_CHUNK * CHUNK_SIZE + CHUNK_SIZE - 1))`"
    OUTPUT_FILE_PATH="$OUTPUT_DIR/$OUTPUT_FILE"
    echo "$line" >> $OUTPUT_FILE_PATH
    COUNT=1
  else
    echo "$line" >> $OUTPUT_FILE_PATH
  fi
done < "$INPUT_FILE"
