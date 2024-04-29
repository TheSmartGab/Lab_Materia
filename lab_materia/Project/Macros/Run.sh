#!/bin/bash

Run_dir=../Run_Data/$1
debug=$2
print=$3

# Directory where the files are located
source_dir="../Lab_Data/"
output_name=Data_names.txt

#Run Executable
Run_exe=./builddir/Run

echo =========================================
echo Run.sh call
echo Run_dir: "$Run_dir"
echo debug "$debug"
echo source_dir "$source_dir"
echo list name "$output_name"
echo =========================================

#checking for updates in source code
#make

# Substring to search for in filenames
substring=$1

# Create the copy directory if it doesn't exist
mkdir -p "$Run_dir"


# Loop over text files in the Data directory
for file in *.txt; do
    # Extract filename without path
    filename=$(basename "$file")
    # Check if the current file is the output file
    if [ "$filename" != "$output_name" ]; then
        # Append filename to output file
        echo "$filename" >> "$output_name"
    fi
done

# Find files containing the substring in their names and copy them to the copy directory
find "$source_dir" -type f -name "*$substring*.txt" ! -name "*out*" -exec cp {} "$Run_dir" \;

echo "Files containing '$substring' in their names have been copied to $Run_dir"


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

Run_exe -Run $1 -print $print -noApp 0 -debug "$debug"

echo "Filenames have been written to $Run_dir/$output_name"