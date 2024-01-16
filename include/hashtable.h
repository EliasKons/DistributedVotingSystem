#pragma once
#include "defines.h"

typedef struct HashNode {
    char name[NAME_SIZE];
    char party[PARTY_SIZE];
    struct HashNode *next;
} HashNode;


typedef struct HashTable {
    HashNode *table[TABLE_SIZE];
} HashTable;


/* Hash the given name. */
int hash(char *name);

/* Initialize hash table ht. */
void init_hashtable(HashTable *ht);

/* Search for the name in ht. */
int search_vote(HashTable *ht, char *name);

/* Insert the key name and the value party in ht. */
void insert_vote(HashTable *ht, char *name, char *party);

/* Free hash table ht. */
void free_hashtable(HashTable *ht);