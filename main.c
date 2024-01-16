#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include "worker.h"
#include "error_handler.h"
#include "signal_handler.h"
#include "defines.h"
#include "info.h"

pthread_mutex_t bufferMutex, dataMutex;
pthread_cond_t bufferNotFull, bufferNotEmpty;
int finish = 0, sock;

int main(int argc, char *argv[]) {
    int port, num_worker_threads, err, newsock, reuse = 1; 
    struct info *t_info;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(client);
    struct sockaddr *serverptr = (struct sockaddr *)&server, *clientptr = (struct sockaddr *)&client;
    pthread_t *workerThreads;
    FILE *pollStats;


    if(argc != 6) {
        printf("Wrong argument usage.\n");
        exit(1);
    }

    port = atoi(argv[1]); // port number of server
    num_worker_threads = atoi(argv[2]); // number of worker-threads

    // information to be passed to the threads
    t_info = malloc(sizeof(struct info)); 
    t_info->size = atoi(argv[3]);
    t_info->buffer = malloc(t_info->size * sizeof(int));
    t_info->count = 0;
    t_info->start = 0;
    t_info->end = -1;
    if((t_info->pollLog = fopen(argv[4], "w")) == NULL) {
        fprintf(stderr, "Error opening %s file\n", argv[4]);
        exit(1);
    }

    // initilization of the votes 
    t_info->votes = malloc(sizeof(HashTable)); 
    init_hashtable(t_info->votes); 

    // initilization of the results
    t_info->results = malloc(sizeof(List));
    init_list(t_info->results);

    
    // initialization of the bufferMutex, dataMutex and the condition variables BufferNotFull, BufferNotEmpty
    pthread_mutex_init(&bufferMutex, NULL);
    pthread_mutex_init(&dataMutex, NULL);
    pthread_cond_init(&bufferNotFull, NULL);
    pthread_cond_init(&bufferNotEmpty, NULL);


    signal(SIGINT, signalHandler); // set signal handler for Ctrl+C


    // threads creation
    workerThreads = malloc(num_worker_threads * sizeof(pthread_t)); 

    for(int i = 0; i < num_worker_threads; i++) {
        if((err = pthread_create(&workerThreads[i], NULL, workerThread, (void*)t_info)) != 0) {
            perror2("pthread_create", err);
            exit(1);
        }
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // socket creation 
        perror_exit("socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) // allowance of reuse for local addresses(used in debugging but could be useful)
        perror_exit("setsockopt");

    if(bind(sock, serverptr, sizeof(server)) < 0) // give socket to the address
        perror_exit("bind");
    
    
    if(listen(sock, SOMAXCONN) < 0) // prepare to accept connections
        perror_exit("listen");
    

    while(!finish) {
        // accept connections 
        if((newsock = accept(sock, clientptr, &clientlen)) < 0) { 
            if(finish == 1) 
                break;
            else {
                perror_exit("accept");
            }
        }

        if(finish)
            break;

        pthread_mutex_lock(&bufferMutex); // lock bufferMutex

        while(t_info->count == t_info->size) // check if buffer is full
            pthread_cond_wait(&bufferNotFull, &bufferMutex); // wait for buffer to not be full

        // bounded buffer server implementation
        t_info->end = (t_info->end + 1) % t_info->size;
        t_info->buffer[t_info->end] = newsock;
        t_info->count++;

        pthread_cond_signal(&bufferNotEmpty); // buffer is not empty

        pthread_mutex_unlock(&bufferMutex); // unlock bufferMutex
    }

    // wait for all the workers to finish
    for(int i = 0; i < num_worker_threads; i++) 
        pthread_join(workerThreads[i], NULL);
    

    fclose(t_info->pollLog); 

    if((pollStats = fopen(argv[5], "w")) == NULL) {
        fprintf(stderr, "Error opening %s file\n", argv[5]);
        return 1;
    }

    print_poll(t_info->results, pollStats); // write results to the file

    fclose(pollStats);

    // destroy mutexes
    pthread_mutex_destroy(&bufferMutex);
    pthread_mutex_destroy(&dataMutex);
    pthread_cond_destroy(&bufferNotFull);
    pthread_cond_destroy(&bufferNotEmpty);

    free(workerThreads);

    free(t_info->buffer);
    
    free_list(t_info->results);
    free(t_info->results);
    free_hashtable(t_info->votes);
    free(t_info->votes);

    free(t_info);

    return 0;
}