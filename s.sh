#!/bin/bash

# Usage: ./concat_files.sh /path/to/source/folder

if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_folder>"
    exit 1
fi

SOURCE_FOLDER="$1"
OUTPUT_FILE="all.c"


if [ ! -d "$SOURCE_FOLDER" ]; then
    echo "Error: Source folder '$SOURCE_FOLDER' does not exist"
    exit 1
fi

> "$OUTPUT_FILE"

echo "Concatenating all files from '$SOURCE_FOLDER' into '$OUTPUT_FILE'..."

find "$SOURCE_FOLDER" -type f \
    -not -path "*/.*" \
    -not -path "*/.vscode/*" \
    -not -path "*/.git/*" \
    -exec sh -c '
    echo "/* ========== File: {} ========== */" >> "$1"
    cat "{}" >> "$1"
    echo "" >> "$1"
    echo "" >> "$1"
' _ "$OUTPUT_FILE" \;

echo "Done! All files have been concatenated into '$OUTPUT_FILE'"
