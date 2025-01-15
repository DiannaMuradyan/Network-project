#include "server.h"

int main()
{
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
    fclose(quiz);
    return 0;
}
