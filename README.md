# LinuxShell
We developed a custom command-line shell in C, similar to Bash, that runs inside a Linux terminal. It supports execution, piping, redirection, and command history — essentially functioning as a lightweight shell environment.

# OS Project – Custom Shell in C

This project is a Linux shell implementation written in C, created as part of an Operating Systems course. It combines modular weekly components (Week 2–6), showing the evolution from basic process creation to advanced shell features like piping, redirection, and command history.

## Features

* Execute basic Linux commands (`ls`, `cat`, `pwd`, etc.)
* Input & output redirection (`>`, `<`, `>>`)
* Command piping (`|`) – connect multiple commands
* Background process execution (`&`)
* Command history (saved across sessions)
* Proper error handling and process synchronization

## File Structure
```
osproject/
├── Makefile
├── main_shell.c          # Central shell program (final integrated version)
├── week2_shell.c         # Basic command execution using fork() & exec()
├── week3_shell.c         # Command parsing and argument handling
├── week4_shell.c         # Input/output redirection
├── week5_shell.c         # Command piping
├── week6_shell.c         # Background processes, history, and error handling
├── fork_demo.c           # Demonstrates fork() usage
├── exec_demo.c           # Demonstrates exec() usage
├── parser_demo.c         # Demonstrates token parsing
├── hello.c               # Test program
└── file.txt              # Example input file
```

## How to Run

### Step 1: Compile the project
```bash
make
```
<img width="1919" height="253" alt="image" src="https://github.com/user-attachments/assets/0728acf0-0d95-4382-a2a6-a7bd79339358" />


### Step 2: Run the shell
```bash
./shell
```

You'll see the prompt:
```
shell@osproject$
```
<img width="1919" height="52" alt="image" src="https://github.com/user-attachments/assets/6a8ddcde-6cf7-4669-b225-0e527933e18c" />


### Example commands:
```bash
ls
cat file.txt
ls | grep .c
ls > output.txt
cat < input.txt | grep "word"
sleep 5 &
```

<img width="1919" height="973" alt="image" src="https://github.com/user-attachments/assets/adc59cfa-60be-43e4-9179-1a231e2bd13a" />



## OS Concepts Used

- **Process Creation** – Using `fork()` to create child processes for command execution
- **Program Execution** – Using `execvp()` to replace the child process with a new program
- **Process Synchronization** – `wait()` and `waitpid()` ensure parent waits for child to finish
- **Pipes** – `pipe()` and `dup2()` connect output of one process to input of another
- **File Descriptors** – Used for input/output redirection with `dup2()`
- **System Calls** – Core system-level functions like `open()`, `read()`, `write()`
- **History Management** – Persistent command history stored in a `.history` file
- **Error Handling** – `perror()` and custom messages for invalid commands

## Functions Explained

| Function | Purpose |
|----------|---------|
| `fork()` | Creates a new child process; both parent and child execute simultaneously |
| `execvp()` | Replaces the child process's memory with a new program (e.g., `ls`, `cat`) |
| `wait()`, `waitpid()` | Makes parent wait until child finishes; avoids zombie processes |
| `pipe()` | Enables command chaining (`ls \| grep .c`) by connecting process I/O |
| `dup2()` | Redirects file descriptors to achieve redirection (`>`, `<`) |
| `open()`, `close()` | Handle file I/O for redirection |
| `strtok()` | Tokenizes user input into arguments |
| `fgets()` | Reads user command safely |
| `perror()` | Prints human-readable system error messages |

## Problems Faced

* Managing multiple child processes and synchronizing them correctly
* Handling multiple pipes and redirection simultaneously
* Preserving shell stability after failed commands
* Debugging file descriptor leaks and ensuring clean I/O
* Maintaining persistent command history across sessions

## Outcome

This project demonstrates how a Unix-like shell works internally — combining system calls, process control, and inter-process communication into one cohesive program.

---

**Author:** Panchal Prince 23BIT169
**Language:** C  
**Platform:** Linux / WSL

