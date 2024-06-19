#!/bin/bash/

NamesFile="Data_names.txt"

for dir in `find . -type d`;
do	
	echo "Clearing $dir/$NamesFile"
	> "$dir/$NamesFile"
	for file in `find $dir -type f -name "*.txt" ! -name "*$NamesFile*" ! -name "*out*" ! -name "*Diff*" ! -name "*Ratio*"`;
	do
		echo adding file $(basename $file)
		echo "$(basename $file)" >> "$dir/$NamesFile"
	

			done
			
done


