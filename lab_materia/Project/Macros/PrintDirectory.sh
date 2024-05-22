#!/bin/bash

path_name=$1
main_file="../builddir/main"


ls "$path_name"


for filename in "$path_name"*.txt; do

    if [[ $filename == *"out"* ]]; then
        continue
    fi
    if [[ $filename == *"diff"* ]]; then
        continue
    fi    
    if [[ $filename == *"Description"* ]]; then
        continue
    fi      
    if [[ $filename == *"Data_names"* ]]; then
        continue
    fi   
    if [[ $filename == *"Thickness_Fitted"* ]]; then
        continue
    fi   

    "$main_file" -name "$(basename "$filename" .txt)" -path "$path_name" -print 1 -noApp 1 -debug 1

done
