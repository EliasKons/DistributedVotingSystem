/* Given an arg, workerThread runs and is responsible for the management of 
the communication with the client. These threads can run at the same time,
so proper synchronization is required. This is done by the use of the mutexes
bufferMutex, dataMutex and the use of the condition variables bufferNotFull
and bufferNotEmpty. bufferMutex and bufferNotEmpty are used to ensure that
the thread start communcating with the client when the buffer is not empty. 
A bounded buffer implemanation for the proper use of the sockets that the 
server creates for the workers. It also signals that the buffer is no longer
full using the bufferNotFull. Finally, with the use of the dataMutex, it sends,
receives and saves necessary information and closes the socket. This is done 
until the finish flag is raised. */
void* workerThread(void* arg);