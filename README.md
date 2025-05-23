# Aggie Shell (ash)

A lightweight Unix-style shell implemented in **C++**. It supports execution of external commands, directory navigation, output redirection, and path configuration—all from a custom shell prompt.

---

## 📁 Project Structure

├── ash.cpp # Source code for the Aggie Shell
└── README.md # Project documentation

---

## 🚀 Features

### 🖥️ Shell Interface
- Displays a custom prompt with the current working directory.
- Parses and executes user commands with or without arguments.

### 🔧 Internal Commands
- `cd <directory>` — Change current working directory.
- `exit` — Exit the shell.
- `path <dir1> <dir2> ...` — Set search paths for executable commands.

### 🔀 Redirection Support
- Supports single output redirection using `>` to write stdout to a file.
- Validates redirection syntax to catch errors (e.g., missing filename, misplaced symbols).

---

## 🛠️ Compilation

Compile the shell using `g++`:

```bash
g++ -o ash ash.cpp
```

---

## ▶️ Running the Shell

Start the shell after compiling:

```bash
./ash
```

---

## 💡 Example Commands

```bash
ls                    # Lists directory contents
ls > output.txt       # Redirects output to a file
cd /tmp               # Changes directory
path /bin /usr/bin    # Sets executable search paths
exit                  # Exits the shell
```

---

## 📌 Notes

- By default, ash uses `/bin` and `/usr/bin` as executable search paths.
- Redirection supports only a single `>` symbol. Multiple or misplaced redirection symbols will cause an error.
- If a command is not found in the specified path(s), an error message will be shown.
- `exit` cleanly terminates the shell session.
- `cd` requires exactly one argument; otherwise, an error is printed.
- Redirection is validated to catch common formatting issues like leading/trailing spaces or invalid symbols.

---

## 📃 License

This project is provided for educational purposes only and does not carry an open-source license.