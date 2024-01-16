#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

extern pthread_mutex_t bufferMutex;
extern pthread_cond_t bufferNotEmpty;
extern int sock, finish;

void signalHandler(int signal) {
    if(signal == SIGINT) {
        pthread_mutex_lock(&bufferMutex); // lock bufferMutex
        finish = 1; // raise flag
        printf("Poll closed. Wait for the rest of the connections to finish their job.");
        close(sock); // close the server's socket
        pthread_cond_broadcast(&bufferNotEmpty); // broadcast to the workers 
        pthread_mutex_unlock(&bufferMutex); // unlock bufferMutex
    }
}
