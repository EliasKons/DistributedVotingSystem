#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "defines.h"


int hash(char *name) {
    int sum = 0, len = strlen(name);

    for(int i = 0; i < len; i++) // sum of the characters as hash
        sum += name[i];
    
    return sum % TABLE_SIZE;
}

void init_hashtable(HashTable *ht) {
    for(int i = 0; i < TABLE_SIZE; i++) 
        ht->table[i] = NULL;
}


int search_vote(HashTable *ht, char *name) {
    int key = hash(name);
    HashNode *current = ht->table[key];
    
    while(current != NULL) {
        if(strcmp(current->name, name) == 0) 
            return 1;
        current = current->next;
    }
    return 0;
}

void insert_vote(HashTable *ht, char *name, char *party) {
    int key = hash(name);
    HashNode *newvote = malloc(sizeof(HashNode)), *current;

    strcpy(newvote->name, name);
    strcpy(newvote->party, party);
    newvote->next = NULL;

    if(ht->table[key] == NULL) 
        ht->table[key] = newvote;
    else {
        current = ht->table[key];
        while(current->next != NULL)
            current = current->next;
        current->next = newvote;
    }
}

void free_hashtable(HashTable *ht) {
    HashNode *current, *temp;
    for(int i = 0; i < TABLE_SIZE; i++) {
        current = ht->table[i];
        while(current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
        ht->table[i] = NULL;
    }
}