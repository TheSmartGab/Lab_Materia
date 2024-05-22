#!/bin/bash

Run_dir=../Run_Data/$1
debug=$2
print=$3

# Directory where the files are located
source_dir="../Lab_Data/"
output_name=Data_names.txt

# Run Executable
Run_exe=./builddir/Run

echo =========================================
echo Run.sh call
echo Run_dir: "$Run_dir"
echo debug "$debug"
echo source_dir "$source_dir"
echo list name "$output_name"
echo =========================================

# Substring to search for in filenames
substring=$1

# Create the copy directory if it doesn't exist
mkdir -p "$Run_dir"

# Find files containing the substring in their names (case-insensitive) and copy them to the copy directory
find "$source_dir" -type f -iname "*$substring*.txt" ! -iname "*out*" -exec bash -c '
for file; do
    # Get the directory and base name of the file
    dir=$(dirname "$file")
    base=$(basename "$file")

    # Transform the base name to have the first letter of the substring capitalized
    new_base=$(echo "$base" | sed "s/$1/\u$1/I")

    # Copy the file with the new name
    cp "$file" "$2/$new_base"
done
' bash {} + "$substring" "$Run_dir"

echo "Files containing '$substring' in their names have been copied to $Run_dir with appropriate name changes."

# Change directory to the specified directory
cd "$Run_dir" || exit

# Clear the output file
> "$output_name"

# Loop over text files in the directory
for file in *.txt; do
    # Extract filename without path
    filename=$(basename "$file")
    # Check if the current file is the output file
    if [[ $filename == *"out"* ]]; then
        continue
    fi
    if [ "$filename" == "Thickness_Fitted.txt" ]; then
        continue
    fi
    if [[ "$filename" == *"Diff"* ]]; then
        continue
    fi
    if [[ "$filename" == *"Ratio"* ]]; then
        continue
    fi

    if [ "$filename" != "$output_name" ]; then
        # Append filename to output file
        echo "$filename" >> "$output_name"
    fi
done

# Change directory back to the original directory
cd - > /dev/null

# Uncomment the line below to run the executable with the provided arguments
# $Run_exe -Run $1 -print $print -noApp 0 -debug "$debug"

echo "Filenames have been written to $Run_dir/$output_name"
