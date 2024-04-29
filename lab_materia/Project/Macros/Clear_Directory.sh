#!/bin/bash

directory="$1"

# Change directory safely
cd "$directory" || exit

# Iterate over all files in the directory
for filename in *; do

    # Check if the filename contains "out" substring
    if [[ $filename == *"out"* ]]; then
        echo "Removing $filename"
        # Uncomment the following line when you're sure it's working as expected
        rm "$filename"
    fi

    # Check if the filename contains ":Zone.Identifier" substring
    if [[ $filename == *":Zone.Identifier"* ]]; then
        echo "Removing $filename"
        # Uncomment the following line when you're sure it's working as expected
        rm "$filename"
    fi

    # Check if the filename contains "dep" or "run" substring
    if [[ $filename == *"dep"* || $filename == *"run"* ]]; then
        new_filename="${filename//dep/Run}"  # Replace "dep" with "Run"
        new_filename="${new_filename//run/Run}"  # Replace "run" with "Run"
        echo "Renaming $filename to $new_filename"
        # Uncomment the following line when you're sure it's working as expected
        mv "$filename" "$new_filename"
    fi

done

# Change back to the previous directory
cd - > /dev/null