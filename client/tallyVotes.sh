#!/bin/bash

inputFile="inputFile"

if [ $# -ne 1 ]; then 
    echo "Error: filename not included."
    exit 1
elif [ ! -f "$inputFile" ]; then 
    echo "InputFile does not exists."
    exit 1
elif [ ! -r "$inputFile" ]; then
    echo "inputFile does not have read permissions."
    exit 1
fi

result="$1"
> $result

# sort the file based on the full name to remove the duplicates, then get the 3rd column, then find the number of the each unique occurence of each party
sort -u -k1,2 "$inputFile" | cut -d ' ' -f 3 | sort | uniq -c | awk '{ print $2, $1 }' > "$result" 

echo "Tally votes created succesfully."