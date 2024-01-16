#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error: text file and/or number of lines not included."
    exit 1
elif [ ! -f  "$1" ]; then 
    echo "First argument must be a text file."
    exit 1
fi

expression='^[0-9]+$' # regex for integer
if ! [[ $2 =~ $expression ]]; then 
    echo "Second argument must be an integer."
    exit 1
fi

# create/override inputFile
touch inputFile 
> inputFile

numLinesParties=$(wc -l < "$1") # number of lines of the parties
numLinesInput=$2 # number of lines of the inputFile


for(( i=0; i < numLinesInput; i++ )); do 
    length=$(( RANDOM % 10 + 3 )) # get random length for the name
    name=$(cat /dev/urandom | tr -dc 'a-zA-Z' | head -c $length) # get random name

    length=$(( RANDOM % 10 + 3 )) # get random length for the lastname
    lastname=$(cat /dev/urandom | tr -dc 'a-zA-Z' | head -c $length) # get random lastname

    randomNumberParties=$(( RANDOM % numLinesParties + 1 )) # get random number of party
    randomParty=$(sed -n "${randomNumberParties}p" "$1") # get the party

    echo "$name $lastname $randomParty" >> inputFile
done

echo "Input file generated successfully."