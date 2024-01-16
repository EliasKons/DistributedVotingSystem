 #!/bin/bash

inputFile="$1"

if [ $# -ne 1 ]; then 
    echo "Error: filename not included."
    exit 1
elif [ ! -f "$inputFile" ]; then 
    echo "$1 does not exists."
    exit 1
elif [ ! -r "$inputFile" ]; then
    echo "$1 does not have read permissions."
    exit 1
fi

result="pollerResultsFile"
> $result

# sort the file based on the full name to remove the duplicates, then get the 3rd column, then find the number of the each unique occurence of each party
sort -u -k1,2 "$inputFile" | cut -d ' ' -f 3 | sort | uniq -c | awk '{ print $2, $1 }' > "$result"

echo "Poller votes created succesfully."