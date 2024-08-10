#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

namespace fs = std::filesystem;
using namespace std;

unordered_map<string, string> builtins = {
    {"echo", "a shell builtin"},
    {"exit", "a shell builtin"},
    {"type", "a shell builtin"},
    {"pwd", "a shell builtin"},
    {"cd", "a shell builtin"}
};

// Function to find the executable path from PATH environment variable
string get_path(const string& command) {
    string path_env = getenv("PATH");
    stringstream ss(path_env);
    string path;
    while (getline(ss, path, ':')) {
        fs::path abs_path = fs::path(path) / command;
        if (fs::exists(abs_path) && fs::is_regular_file(abs_path) &&
            (fs::status(abs_path).permissions() & fs::perms::owner_exec) != fs::perms{}) {
            return abs_path.string();
        }
    }
    return ""; // Command not found
}

void handle_cd(const string& path) {
    // Handle the ~ character
    string resolved_path = path;
    if (resolved_path[0] == '~') {
        // Replace ~ with the home directory
        string home_dir = getenv("HOME");
        resolved_path = home_dir + resolved_path.substr(1);
    }

    // Change directory
    if (fs::exists(resolved_path) && fs::is_directory(resolved_path)) {
        fs::current_path(resolved_path);
    } else {
        cout << "cd: " << path << ": No such file or directory\n";
    }
}

int main() {
    cout << unitbuf;

    string input;
    while (true) {
        cout << "$ ";
        getline(cin, input);

        if (input.empty()) {
            continue;
        }

        // Handle exit command
        if (input.find("exit") == 0) {
            istringstream iss(input);
            string command;
            int exit_code = 0;
            iss >> command >> exit_code;
            exit(exit_code);
        }

        // Handle echo command
        if (input.find("echo") == 0) {
            size_t pos = input.find(" ");
            if (pos != string::npos) {
                string message = input.substr(pos + 1);
                cout << message << endl;
            }
            continue;
        }

        // Handle pwd command
        if (input == "pwd") {
            string cwd = fs::current_path().string();
            cout << cwd << endl;
            continue;
        }

        // Handle type command
        if (input.find("type") == 0) {
            size_t pos = input.find(" ");
            if (pos != string::npos) {
                string cmd = input.substr(pos + 1);
                auto it = builtins.find(cmd);
                if (it != builtins.end()) {
                    cout << cmd << " is " << it->second << endl;
                } else {
                    string path = get_path(cmd);
                    if (path.empty()) {
                        cout << cmd << ": not found" << endl;
                    } else {
                        cout << cmd << " is " << path << endl;
                    }
                }
            }
            continue;
        }

        // Handle cd command
        if (input.find("cd") == 0) {
            size_t pos = input.find(" ");
            if (pos != string::npos) {
                string path = input.substr(pos + 1);
                handle_cd(path);
            }
            continue;
        }

        // Handle external commands
        istringstream iss(input);
        string command;
        iss >> command;
        vector<string> args;
        string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        string full_path = get_path(command);

        if (full_path.empty()) {
            cout << command << ": command not found" << endl;
        } else {
            // Convert command and arguments to array of char*
            vector<char*> argv;
            argv.push_back(const_cast<char*>(full_path.c_str()));
            for (const auto& a : args) {
                argv.push_back(const_cast<char*>(a.c_str()));
            }
            argv.push_back(nullptr); // execvp expects a null-terminated array

            // Fork the process
            pid_t pid = fork();
            if (pid == 0) {
                execvp(argv[0], argv.data());
                perror("execvp failed");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                perror("fork failed");
            }
        }
    }
    return 0;
}
