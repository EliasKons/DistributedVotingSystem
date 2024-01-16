#pragma once
#include "defines.h"

typedef struct ListNode {
    char party[PARTY_SIZE];
    int votes;  
    struct ListNode *next;
} ListNode;

typedef struct List {
    ListNode *head;
} List;


/* Initialize List list. */
void init_list(List *list);

/* Insert party to the list. */
void add_vote(List *list, char *party);

/* Print the list to the file. */
void print_poll(List *list, FILE *file);

/* Free List list. */
void free_list(List *list);