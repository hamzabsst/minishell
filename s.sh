#!/bin/bash
# Linux/macOS version - concat_files.sh
# Usage: ./concat_files.sh /path/to/source/folder

if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_folder>"
    exit 1
fi

SOURCE_FOLDER="$1"
OUTPUT_FILE="all.c"

# Check if source folder exists
if [ ! -d "$SOURCE_FOLDER" ]; then
    echo "Error: Source folder '$SOURCE_FOLDER' does not exist"
    exit 1
fi

# Clear the output file
> "$OUTPUT_FILE"

echo "Concatenating all files from '$SOURCE_FOLDER' into '$OUTPUT_FILE'..."

# Find all files (not directories) and concatenate them, excluding hidden files/folders, .vscode, and .git
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
