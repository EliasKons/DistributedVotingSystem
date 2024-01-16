#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include "defines.h"


void init_list(List *list) {
    list->head = NULL;
}

void add_vote(List *list, char *party) {
    ListNode *current = list->head, *newnode;

    while(current != NULL) {
        if(strcmp(current->party, party) == 0) {
            current->votes++;
            return;
        }
        current = current->next;
    }

    newnode = malloc(sizeof(ListNode));

    strcpy(newnode->party, party);
    newnode->votes = 1;
    newnode->next = NULL;

    if(list->head == NULL)
        list->head = newnode;
    else {
        current = list->head;
        while(current->next != NULL)
            current = current->next;
        current->next = newnode;
    }
}

void print_poll(List *list, FILE *file) {
    ListNode *current = list->head;
    while(current != NULL) {
        fprintf(file, "%s %d\n", current->party, current->votes);
        current = current->next;
    }
}


void free_list(List *list) {
    ListNode *current = list->head, *temp;
    while(current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}