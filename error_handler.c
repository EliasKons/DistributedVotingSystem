#include <stdio.h>
#include <stdlib.h>


void perror_exit(char *message) {
    perror(message);
    exit(1);
}