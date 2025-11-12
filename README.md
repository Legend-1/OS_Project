# LinuxShell
We developed a custom command-line shell in C, similar to Bash, that runs inside a Linux terminal. It supports execution, piping, redirection, and command history — essentially functioning as a lightweight shell environment.  

Now, an additional **GUI interface** (built using PyQt5) allows users to run shell commands through an interactive, customizable desktop app.

---

# OS Project – Custom Shell in C (with GUI)

This project is a Linux shell implementation written in C, created as part of an Operating Systems course.  
It combines modular weekly components (Week 2–6) showing the evolution from basic process creation to advanced shell features like piping, redirection, and history — and now, includes a **graphical user interface** for enhanced usability.

---

## Features

### 🧠 Core Shell Features
* Execute basic Linux commands (`ls`, `cat`, `pwd`, etc.)
* Input & output redirection (`>`, `<`, `>>`)
* Command piping (`|`)
* Background process execution (`&`)
* Command history (persistent across sessions)
* Proper error handling and process synchronization  

### 🖥️ GUI Features
* Developed using **PyQt5**
* Real-time communication with shell backend (via sockets)
* Adjustable **font size**, **font style**, and **color themes**
* Scrollable command output panel
* Command input box with Enter key binding
* Status bar showing connection & process states
* Option to switch between **Light** and **Dark** modes  

---

## File Structure

```
osproject/
├── Makefile
├── main_shell.c              # Central shell program (final integrated version)
├── main_shell_server.c       # Socket-based server to connect GUI ↔ shell
├── os_terminal_gui/
│   ├── os_terminal_gui.py    # PyQt5-based GUI client
│   └── assets/               # Optional icons or theme files
├── week2_shell.c             # Basic command execution using fork() & exec()
├── week3_shell.c             # Command parsing and argument handling
├── week4_shell.c             # Input/output redirection
├── week5_shell.c             # Command piping
├── week6_shell.c             # Background processes, history, error handling
├── fork_demo.c               # Demonstrates fork() usage
├── exec_demo.c               # Demonstrates exec() usage
├── parser_demo.c             # Demonstrates token parsing
└── hello.c                   # Test program
```

---

## How to Run

### Step 1: Compile the Shell Server
```bash
gcc main_shell_server.c -o shell_server -ljson-c
./shell_server
```

### Step 2: Launch the GUI in virtual environment
```bash
source ~/pyqt_env/bin/activate
python3 os_terminal_gui/os_terminal_gui.py
```

The GUI will automatically connect to the backend server and allow you to execute Linux commands visually.

---

### 🖼️ GUI Preview While Running

Below are snapshots of the GUI version of **LinuxShell** in action:

#### ▶️ GUI Home
<img width="903" height="679" alt="image" src="https://github.com/user-attachments/assets/e983970d-c9df-4b46-a263-6c16195ba9a3" />

#### 💻 Running Commands
<img width="902" height="680" alt="image" src="https://github.com/user-attachments/assets/2a57f75c-b5fe-4ace-a514-daa6e874f42a" />

#### 🎨 Customization (Font / Dark Theme)
<img width="904" height="679" alt="image" src="https://github.com/user-attachments/assets/fbd4b56b-fa52-4077-a61e-01015fb00a97" />

---

## OS & GUI Concepts Used

| Concept | Description |
|----------|--------------|
| **Process Creation** | Using `fork()` to create child processes for each command |
| **Program Execution** | `execvp()` replaces child’s memory with new program |
| **Sockets** | Enables client-server communication between GUI and shell backend |
| **Event Handling (PyQt)** | Reacts to user input, button clicks, and text entry |
| **Multithreading (Python)** | Keeps GUI responsive during long command execution |
| **File Descriptors** | Used for redirection and piping (`dup2()`, `pipe()`) |
| **Custom GUI Styling** | Dynamic font color, background, and theme switching |

---

## Outcome

This version of **LinuxShell** provides both a **command-line** and a **graphical interface**, demonstrating:
- Deep understanding of process management & inter-process communication in C  
- Real-world integration of system programming and GUI development  
- Extensibility of OS-level tools into user-friendly applications  

---

**Authors:**  
Harsh Varia & Panchal Prince  

**Languages:**  
C, Python (PyQt5)  

**Platform:**  
Linux / WSL  

---


