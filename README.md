# README: Shell

This README provides an in-depth explanation of the implementation of various shell commands in our custom shell. 

## Exit Command

- **Purpose:** To terminate the shell and optionally specify an exit status.
- **Implementation:** 
  - The command is identified by checking if the input starts with "exit".
  - The `exit` command can optionally take an integer argument for the exit status.
  - The `exit()` function is used to terminate the shell process with the specified exit status.

### Libraries and Functions Used
- `<cstdlib>`: Provides the `exit()` function to terminate the program with a specific exit status.

---

## Echo Command


- **Purpose:** To print the provided message to the standard output.
- **Implementation:** 
  - The command is identified by checking if the input starts with "echo".
  - The message to be printed is extracted from the input string after the "echo" keyword.
  - The `std::cout` stream is used to output the message followed by a newline.

### Libraries and Functions Used
- `<iostream>`: Provides the `std::cout` stream for outputting text to the console.

---

## Pwd Command


- **Purpose:** To print the current working directory.
- **Implementation:** 
  - The command is identified by checking if the input exactly matches "pwd".
  - The `std::filesystem::current_path()` function is used to get the current directory.
  - The `std::cout` stream is used to print the directory path.

### Libraries and Functions Used
- `<filesystem>`: Provides the `std::filesystem::current_path()` function to retrieve the current working directory.
- `<iostream>`: Provides the `std::cout` stream for outputting text to the console.

---

## Type Command


- **Purpose:** To determine whether a command is a built-in or an external executable.
- **Implementation:** 
  - The command is identified by checking if the input starts with "type".
  - The `std::unordered_map` is used to map built-in commands to their descriptions.
  - For non-built-in commands, the `get_path()` function is used to find the executable path from the `PATH` environment variable.
  - The result is printed indicating whether the command is a built-in or an executable file.

### Libraries and Functions Used
- `<unordered_map>`: Used to map built-in commands to their descriptions.
- `<sstream>`: Used for parsing the input command to extract the command name.
- `<filesystem>`: Used in `get_path()` function to check the existence of files.
- `<cstdlib>`: Provides `std::getenv()` to access the `PATH` environment variable.
- `<iostream>`: Provides the `std::cout` stream for outputting text to the console.

---

## Cd Command


- **Purpose:** To change the current working directory.
- **Implementation:** 
  - The command is identified by checking if the input starts with "cd".
  - Paths are handled in three ways:
    - **Absolute Paths:** Directly set the `WORKING_DIR` to the specified path if it exists and is a directory.
    - **Relative Paths:** Handled in future stages (not implemented in this step).
    - **Home Directory (`~`):** Translates `~` to the home directory specified by the `HOME` environment variable.
  - For successful directory changes, `std::filesystem::current_path()` is updated to the new directory.

### Libraries and Functions Used
- `<filesystem>`: Provides `std::filesystem::exists()` to check if the directory exists and `std::filesystem::is_directory()` to check if it's a directory.
- `<cstdlib>`: Provides `std::getenv()` to access the `HOME` environment variable for resolving the home directory.
- `<iostream>`: Provides the `std::cout` stream for outputting errors and status messages.

---

## External Commands Execution


- **Purpose:** To execute commands that are not built-in.
- **Implementation:** 
  - The executable path is determined using the `get_path()` function.
  - The command and arguments are prepared and passed to `fork()` to create a new process.
  - The `execvp()` function replaces the current process image with the new command.
  - The parent process waits for the child process to finish using `waitpid()`.

### Libraries and Functions Used
- `<vector>`: Used to store command arguments and convert them to a format suitable for `execvp()`.
- `<unistd.h>`: Provides `fork()` for process creation and `execvp()` for executing the command.
- `<sys/wait.h>`: Provides `waitpid()` to wait for the child process to terminate.

---
