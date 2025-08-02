# 🐚 minishell

A minimal Unix shell implementation built in C, replicating key behavior of Bash. Developed for educational purposes.

## ✅ Features

- **Command execution** with arguments (e.g., `ls -l`, `echo hello`)
- **Built-in commands:**
  - `cd`, `echo`, `pwd`, `exit`, `export`, `unset`, `env`
- **Input parsing** with support for:
  - Quoting (`'`, `"`)
  - Environment variable expansion (`$VAR`)
  - Redirections (`>`, `>>`, `<`, `<<`)
  - Pipes (`|`)
- **Signal handling** (Ctrl+C, Ctrl+\, Ctrl+D)
- **Error handling** with descriptive messages
- **No memory leaks** (checked with valgrind)

## 🛠️ Requirements

- Unix-based OS (Linux or macOS)
- `cc` compiler
- `make`

## ▶️ Usage

```bash
./minishell
```

You can now enter commands like:

```bash
ls -l | grep ".c" > files.txt
cat < files.txt
```

Use `exit` to quit the shell.

## 📁 Project Structure

```
minishell/
├── src/                # Source files
├── inc/                # Header files
├── mylib/              # Custom standard library functions
├── Makefile
└── README.md
```

## 🧪 Testing

- Run `valgrind ./minishell` to check for memory leaks
- Compare behavior with `/bin/bash` for edge cases

## 📚 Reference

This project follows the specifications from the 42 School minishell subject. It mimics Bash 5.1 behavior wherever possible.
