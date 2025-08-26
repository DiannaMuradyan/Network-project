# 📝 Server-Clients Quiz Game in C

The **Server-Clients Quiz Game** is a networked application written in **C** that allows multiple clients to connect to a server and play a multiple-choice quiz.  

The server reads questions from a `quiz.txt` file, sends them to the connected clients, evaluates their answers, and then sends back the total score.  

This project demonstrates concepts of **socket programming**, **multithreading**, and **file handling** in C.

---

## 🚀 Features

- Server handles multiple clients simultaneously using **multithreading**.  
- Questions and answers stored in `quiz.txt` (customizable).  
- Supports **multiple-choice questions (MCQs)**.  
- Questions and answers are parsed using delimiters:
  - `/` separates question from answer choices.  
  - `,` separates individual answer choices.  
- Server randomly selects a set number of questions for each client (e.g., 5).  
- Final score is calculated and sent back to the client.  
- Clients can run on the same machine or across different machines on the same network.  

---

## 📂 Project Structure
```
.
├── server.c        # Server logic (sockets, threading, quiz handling)
├── servermain.c    # Main entry point for server
├── client.c        # Client logic (connect, send answers, receive results)
├── clientmain.c    # Main entry point for client
├── quiz.txt        # Quiz questions & answers (editable)
└── README.md       # Documentation
```

---

## 🏗️ Compilation

Open two different terminals for server and client.  

### Compile Server
```bash
gcc -o server server.c servermain.c -lpthread
gcc -o client client.c clientmain.c

By default, the client connects to 127.0.0.1 (localhost).
To connect from another PC on the same network:
Run ifconfig (Linux/Mac) or ipconfig (Windows) on the server machine.
Find the local IP address (e.g., 192.168.x.x).
Update the IP address in client.c before compiling.
```
