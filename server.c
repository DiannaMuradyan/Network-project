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

int main() {
    quiz = fopen("quiz.txt", "r");
    if (quiz == NULL) {
        perror("fopen quiz.txt");
        exit(1);
    }

    stat("quiz.txt", &st);

    file = (char*)malloc(st.st_size * sizeof(char));
    if (file == NULL) {
        perror("Failed to allocate memory for file pointer");
        exit(1);  
    }

    fread(file, 1, st.st_size, quiz);

    fseek(quiz, 0, SEEK_SET);

    questions_count();

    q_arr = (questions*)malloc(count * sizeof(questions));
    if (q_arr == NULL) {
        perror("Failed to allocate memory for questions array");
        exit(1);  
    }

    parsing_questions();
    parsing_answers();

    srand(time(NULL));

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(1);
    }

    int optval = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
        perror("sockopt_addr");
        exit(1);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval))) {
        perror("sockopt_port");
        exit(1);
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    socklen_t size;
    int bind_s = bind(socket_fd, (struct sockaddr*)&server, sizeof(server)); 
    if (bind_s == -1) {
        perror("bind");
        exit(1);
    }

    int lis = listen(socket_fd, SOMAXCONN);
    if (lis == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server has started\nEvery client will be given 5 questions.\nAt the end of the Quiz, Server will show the scores.\n\n");
    printf("Instruction: To terminate the Server click Ctrl C\n");

    while (true) {
        struct sockaddr client;
        memset(&client, 0, sizeof(client));

        socklen_t len;
        user[i].fd = accept(socket_fd, &client, &len);
        if (user[i].fd < 0) {
            perror("accept");
            exit(1);
        }
        pthread_create(&user[i].th, NULL, server_thread, (void*)(long)i);
        ++i;
    }

    for (int j = 0; j < i; ++j) {
        pthread_join(user[i].th, NULL);
    }

    free(file);
    free(q_arr);
    free(answers);
    return 0;
}

