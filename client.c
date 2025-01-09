#include "client.h"

int socket_fd;

void* reading(void* args) {
    char buf[MAX_BUF_SIZE];
    int ind = 0;
    while (ind < INDEX) {
        memset(buf, 0, MAX_BUF_SIZE);
        if (read(socket_fd, buf, MAX_BUF_SIZE) < 0) {
            perror("connection lost");
            exit(1);
        }
        printf("%s\n", buf);
        ++ind;
    }
    return NULL;
}


void* writing(void* args) {
    char buf[MAX_BUF_SIZE];
    int ind = 0;
    while (ind < INDEX - 1) {
        memset(buf, 0, MAX_BUF_SIZE);
        fgets(buf, MAX_BUF_SIZE, stdin);
        if (write(socket_fd, buf, strlen(buf)) < 0) {
            perror("connection lost");
            exit(1);
        }
        ++ind;
    }
    return NULL;
}
