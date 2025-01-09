#include "server.h"

struct stat st;
FILE* quiz;
char *file;
int count;
questions *q_arr;
th_create user[MAX_CLIENT_COUNT];
int i;
char* answers;
char* token;


void questions_count() {
    for (int i = 0; i < strlen(file); ++i) {
        if (file[i] == '/') {
            ++count;
        }
    }
    return;
}

void parsing_questions() {
    token = file; 
    answers = (char*)malloc(strlen(file) * sizeof(char));
    int ind = 0;
    while ((token = strtok(token, "/"))) {
        strncpy(q_arr[ind++].quest, token, strlen(token));
        answers = token;
        token = strtok(NULL, "\0");
    }
}

void parsing_answers() {
    int ind = 0;
    while ((answers = strtok(answers, ","))) {
        strncpy(q_arr[ind].answer, answers, strlen(answers));
        ++ind;
        answers = strtok(NULL, "\0");
    }
}

void* server_thread(void* args) {
    int id = (int)(long)args;
    char command[MAX_BUF_SIZE];
    int q_number = 0;
    volatile int score = 0;
    memset(command, 0, MAX_BUF_SIZE);
    
    // Reading the command from the client
    if (read(user[id].fd, command, MAX_BUF_SIZE) <= 0) {
        perror("read error");
        pthread_exit(NULL);
    }

    if (strncmp(command, "START", strlen("START")) == 0) {
        int q_count = 0;
        char ans[20];
        while (q_count < CLIENT_QUEST_COUNT) {
            memset(ans, 0, 20);
            q_number = rand() % count;
            if (write(user[id].fd, q_arr[q_number].quest, strlen(q_arr[q_number].quest)) <= 0) {
                perror("lost connection with client");
                pthread_exit(NULL);
            }
            if (read(user[id].fd, ans, 20) <= 0) {
                perror("lost connection with client");
                pthread_exit(NULL);
            }

            if (strncmp(q_arr[q_number].answer, ans, strlen(q_arr[q_number].answer)) == 0) {
                ++score;
            }
            ++q_count;
        }
    }

    score += 48;
    write(user[id].fd, "Your score : ", strlen("Your score : "));
    write(user[id].fd, (char*)&score, 1);
    return NULL;
}


