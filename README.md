# DistributedVotingSystem

## Compile and Run

### Server

- Build: `make`
- Build and Run: `make run`
- Build and Run with Valgrind: `make valgrind`
- Clean: `make clean`

### Client

- Build: `make`
- Build and Run: `make run`
- Build and Run with Valgrind: `make valgrind`
- Clean: `make clean`

### Scripts

Scripts for client operations:

- `create_input.sh` and `tallyVotes.sh` are in the `Client` folder.
- `processLogFile.sh` is in the main folder.

## Server

### `main` Function

The `main` function initializes the necessary structures for the server, such as the buffer, hash table for votes, and results list. It opens the specified file, initializes mutexes (`bufferMutex`, `dataMutex`) and condition variables (`bufferNotFull`, `bufferNotEmpty`) for thread synchronization. It sets up a signal handler for `SIGINT` to terminate the program gracefully, creates threads, and starts the server. The server creates a socket, accepts connections, and handles client requests. If a `Ctrl+C` signal is received, the server stops accepting connections, waits for worker threads, writes results to the appropriate file, and frees dynamically allocated memory.

### `worker` Function

Worker threads lock the `bufferMutex`, check if the buffer is empty, find the next socket, update buffer-related variables, and signal that the buffer is no longer full using `BufferNotFull`. They then send and receive messages using read commands, write to and update the structures using `dataMutex`, and close the socket.

### ADTs

Simple implementations of linked list and separate chaining hash table with creation, addition, destruction, and search functions, adapted to the context of the program.

## Client

The client creates threads equal to the number of lines in the file, providing them with the server name, socket number, and the line they are assigned to process.

### Client Threads

Client threads find the first name and comma, create sockets, and connect to the server. They read and write appropriate messages, then terminate.

## Usage

1. Compile the server and client as mentioned above.
2. Run the server and client binaries.
3. Use scripts (`create_input.sh`, `tallyVotes.sh`, `processLogFile.sh`) as needed for additional operations.
