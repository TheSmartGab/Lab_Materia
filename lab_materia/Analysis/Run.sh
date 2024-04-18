#!/bin/bash

Run_dir=../Run_Data/$1

# Directory where the files are located
source_dir="../Lab_Data/"

# Substring to search for in filenames
substring=$1

# Create the copy directory if it doesn't exist
mkdir -p "$Run_dir"

# Find files containing the substring in their names and copy them to the copy directory
find "$source_dir" -type f -name "*$substring*.txt" ! -name "*out*" -exec cp {} "$Run_dir" \;

echo "Files containing '$substring' in their names have been copied to $copy_dir"



output_name=Data_names.txt

# Change directory to the specified directory
cd "$Run_dir" || exit

# Clear the output file
> "$output_name"

# Loop over text files in the directory
for file in *.txt; do
    # Extract filename without path
    filename=$(basename "$file")
    # Check if the current file is the output file
    if [ "$filename" != "$output_name" ]; then
        # Append filename to output file
        echo "$filename" >> "$output_name"
    fi
done

# Change directory back to the original directory
cd - > /dev/null

./Run -Run $1 -print 1 -noApp 1

echo "Filenames have been written to $Run_dir/$output_name"