# Server-Clients Quiz Game in C
The Client-Server Quiz Game is a networked application developed in the C programming language. It consists of a server that hosts a quiz by sending questions to connected clients, and clients that interact with the server by answering these questions. The quiz includes multiple-choice  questions. Questions and answers are separated by delimiters` "/" and "," for easy parsing .You can add as mush questions as you want in quiz.txt. The server randomly selects a specified number of questions for each client (e.g., five questions), then the server calculates and sends the client's total score. This project enhances understanding of network programming, file handling, and multithreading in C.

P.S.
Use gcc -o server  server.c servermain.c and  gcc -o client client.c clientmain.c in different terminals.
if you want to connect with another PC as client,change the lookback IP address- 127.0.0.1 to your local IP address(prompt ifconfig in terminal to see your local IP address).
