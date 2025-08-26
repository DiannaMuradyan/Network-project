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

