#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>

#define PORT 3367
#define MAX_BUF_SIZE 1024
#define MAX_CLIENT_COUNT 10
#define CLIENT_QUEST_COUNT 5
#define MAX_QUEST_SIZE 1024

// Struct to hold the thread information for each client
typedef struct {
    pthread_t th;
    int fd;
} th_create;

// Struct for questions and answers
typedef struct {
    char quest[MAX_QUEST_SIZE];
    char answer[20];
} questions;

extern struct stat st;
extern FILE* quiz;
extern char *file;
extern int count;
extern questions *q_arr;
extern th_create user[MAX_CLIENT_COUNT];
extern int i;
extern char* answers;
extern char* token;

// Function declarations
void questions_count(void);
void parsing_questions(void);
void parsing_answers(void);
void* server_thread(void* args);

#endif // SERVER_H

