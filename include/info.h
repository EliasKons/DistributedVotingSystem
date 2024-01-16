#pragma once
#include <stdio.h>
#include "linkedlist.h"
#include "hashtable.h"

struct info {
    int *buffer, size, count, start, end;
    HashTable *votes;
    List *results;
    FILE *pollLog;
};
