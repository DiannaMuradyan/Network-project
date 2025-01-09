#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>

#define PORT 3367
#define MAX_BUF_SIZE 1024
#define INDEX 6

extern int socket_fd;

// Function declarations
void* reading(void* args);
void* writing(void* args);

#endif // CLIENT_H

