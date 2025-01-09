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

int main() {
    pthread_t th_write, th_read;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    int optval = 1;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
        perror("sockopt_addr");
        exit(1);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval))) {
        perror("sockopt_port");
        exit(1);
    }

    int con = connect(socket_fd, (struct sockaddr*)&client, sizeof(client));
    if (con < 0) {
        perror("connect");
        exit(1);
    }

    char command[MAX_BUF_SIZE];
    memset(command, 0, sizeof(command));

    printf("You will be given 5 questions during the quiz.\nAt the end you will see your score.\nGood luck!\n\n");
    printf("Please enter START to start the quiz!\n");

    fgets(command, sizeof(command), stdin);

    if (write(socket_fd, command, strlen(command)) < 0) {
        perror("write");
        exit(1);
    }

    pthread_create(&th_write, NULL, writing, NULL);
    pthread_create(&th_read, NULL, reading, NULL);

    pthread_join(th_write, NULL);
    pthread_join(th_read, NULL);

    return 0;
}

