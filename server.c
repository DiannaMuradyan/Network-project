//SERVER
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


#define PORT 3367
#define MAX_BUF_SIZE 1024
#define MAX_CLIENT_COUNT 10
#define Q_COUNT 8
#define CLIENT_QUEST_COUNT 5
#define MAX_QUEST_SIZE 200

typedef struct{
    pthread_t th;
    int fd;
}th_create;

typedef struct{
    char quest[MAX_QUEST_SIZE];
    char answer[20];
}questions;

FILE* quiz;
char file[1024];
th_create user[MAX_CLIENT_COUNT]; 
int i;
char* answers;

questions q_arr[Q_COUNT];

void parsing_questions(){
    char* token = file;
    int ind=0;
    while ((token = strtok(token,"/"))){
        strncpy(q_arr[ind].quest,token,strlen(token));
	answers = token;
        ++ind;
        token = strtok(NULL,"\0");
    }
}


void parsing_answers(){
    char* token = answers;
    int ind=0;
    while ((token = strtok(token,","))){
     	strncpy(q_arr[ind].answer,token,strlen(token));
        ++ind;
        token = strtok(NULL,"\0");
    }
}



void* server_thread(void* args){
    int id = (int)(long)args;
    char command[MAX_BUF_SIZE];
    int q_number = 0;
    volatile int score = 0;
    memset(command,0,MAX_BUF_SIZE);
    read(user[id].fd,command,MAX_BUF_SIZE);
    
    if(strncmp(command,"START",strlen("START")) == 0)
    {
        int q_count = 0;
        char ans[20];
        while(q_count < CLIENT_QUEST_COUNT)
        {
            memset(ans,0,20);
            q_number = rand()%5;
            if(write(user[id].fd,q_arr[q_number].quest,strlen(q_arr[q_number].quest)) <= 0)
            {
                perror("connection lost client");
                pthread_exit(NULL);
            }
            if(read(user[id].fd,ans,20) <= 0)
            {
                perror("connection lost client");
                pthread_exit(NULL);
            }

            if(strncmp(q_arr[q_number].answer,ans,strlen(token) == 0)
            {
                ++score;
            }
            ++q_count;
        }
    }
    else
    {
        printf("You enterd wrong command\n");
    }
    score += 48;
    write(user[id].fd,"Your score : ",strlen("Your score : "));
    write(user[id].fd,(char*)&score,1);
    return NULL;
}


int main()
{
    quiz = fopen("quiz.txt","r");
    if(quiz == NULL)
    {
        perror("fopen quiz.txt");
        exit(1);
    }

    fread(file,1,sizeof(file),quiz);
    fseek(quiz,0,SEEK_SET);

    parsing_questions();
    parsing_answers();

    srand(time(NULL));

    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd == -1)
	{
	    perror("failed socket : ");
	    exit(1);
	}

    int optval = 1;

    if(setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)))
    {
        perror("sockopt_addr");
        exit(1);
    }

    if(setsockopt(socket_fd,SOL_SOCKET,SO_REUSEPORT,&optval,sizeof(optval)))
    {
        perror("sockopt_port");
        exit(1);
    }


	struct sockaddr_in server;
	memset(&server,0,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	socklen_t size;

	int bind_s = bind(socket_fd,(struct sockaddr*)&server,sizeof(server)); 
	if(bind_s == -1)
		{
			perror("failed bind : ");
			exit(1);
		}	
	int lis = listen(socket_fd,SOMAXCONN);
	if(lis == -1)
		{
			perror("failed listen");
			exit(1);
		}

    printf("Server has started\nEvery client will be given 5 questions.\nAt the end of the Quiz Server will show the scores.\n\n");
    printf("Instruction : To terminate the Server click Ctrl C\n");


    while(true){
        struct sockaddr client;
        memset(&client,0,sizeof(client));

        socklen_t len;
        user[i].fd = accept(socket_fd,&client,&len);
        if(user[i].fd < 0)
        {
            perror("accept");
            exit(1);
        }
        pthread_create(&user[i].th,NULL,server_thread,(void*)(long)i);
        ++i;
    }

    for(int j=0;j < i ;++j)
    {
        pthread_join(user[i].th,NULL);
    }	
}
