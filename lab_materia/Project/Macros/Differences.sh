#!/bin/bash

# Directory path containing the text files
directory="../Run_Data/Run_2"

# Iterate over each file in the directory
for file in "$directory"/*.txt; do
    # Check if the file name contains "RERun"
    if [[ $file == *"out"* ]]; then 
        continue
    fi
    if [[ $file == *"Diff"* ]]; then
        continue
    fi
    
    if [[ $file == *"RERun"* ]]; then
        # Extract the number or letter after "vetro_oro_"
        name1=$(echo "$file" | grep -oP '(?<=vetro_oro_)[0-9]{1,2}|(?<=vetro_oro_)[a-zA-Z]')
        filename=$(basename "$file")
        filename_no_ext="${filename%.*}"
        # Find the corresponding "name2" file
        name2_file=$(find "$directory" -maxdepth 1 -type f -name "*vetro_oro_${name1}_*" -not -name "*RERun*" | xargs -I{} basename {} | sed 's/\.[^.]*$//')
        # Run the command with the obtained names
        if [ -n "$name2_file" ]; then
            ../builddir/Difference -path "$directory/" -name1 "$filename_no_ext" -name2 "$name2_file" -print 1 -noApp 1 -debug 1
        else
            echo "Error: Matching file not found for $file"
        fi
    fi
done
