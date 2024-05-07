#!/bin/bash

path_name="../Run_Data/Run_2/"
main_file="../builddir/main"

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
    if [[ $filename == *"Data_nanes"* ]]; then
        continue
    fi   
    $main_file -name $(basename "$filename" .txt) -path $path_name -print 1 -noApp 1

done