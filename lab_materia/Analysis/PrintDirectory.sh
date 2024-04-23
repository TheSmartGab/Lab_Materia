#!/bin/bash

path_name="../Lab_Data/16_04_24/"

for filename in $path_name*.txt; do

    if [[ $filename == *"out"* ]]; then
        continue
    fi
    if [[ $filename == *"diff"* ]]; then
        continue
    fi    
    if [[ $filename == *"Description"* ]]; then
        continue
    fi        
    ./main -name $(basename "$filename" .txt) -path $path_name -print 1 -noApp 1

done