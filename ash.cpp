/*
Name: Jayden Minor
Date: 4/10-13/2025
Class: Comp 350 Section 1
Program: ash (Aggie Shell)
-- Basic shell [Checkpoint 3]
   Updated executeCommand and processCommand to allow
   redirection and ability to execute cmds w/ or w/o params
   # added some error handling for redirection cmds (personal addition /  bonus)
*/
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

// Default search paths
vector<string> searchPaths;

// Function to get the current directory and format the shell prompt
string getPrompt() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return "ash " + string(cwd) + " > ";
    }
    return "ash > ";
}

// Function to execute external commands
#include <fcntl.h> // For open()
#include <cstring> // For strerror

// Function to execute external commands w/ optional redirection
void executeCommand(const vector<string> &args, const string &redirectFile = "") {
    for (const string &path : searchPaths) {
        string fullPath = path + "/" + args[0];
        if (access(fullPath.c_str(), X_OK) == 0) {
            pid_t pid = fork();
            if (pid == 0) { // Child
                if (!redirectFile.empty()) {
                    int fd = open(redirectFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) {
                        cerr << "Error opening file for redirection: " << strerror(errno) << endl;
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
                    close(fd);
                }

                vector<char*> cargs;
                for (const string &arg : args)
                    cargs.push_back(const_cast<char*>(arg.c_str()));
                cargs.push_back(NULL);
                execv(fullPath.c_str(), cargs.data());
                exit(1);
            } else if (pid > 0) { // Parent
                waitpid(pid, NULL, 0);
                return;
            } else {
                cerr << "Failed to fork process." << endl;
                return;
            }
        }
    }
    cout << "Command not found: " << args[0] << endl;
}

// Function to process and route user input
void processCommand(const string &command) {
    if (command.empty()) {
        cout << "Please enter a valid command." << endl;
        return;
    }

    // Handle redirection
    string cmdPart = command;
    string redirectFile = "";

    // Count the number of '>' symbols
    size_t redirCount = count(command.begin(), command.end(), '>');

    if (redirCount > 1) {
        cout << "Redirection error: Multiple redirection symbols are not supported." << endl;
        return;
    }

    size_t redirPos = command.find('>');
    if (redirPos != string::npos) {
        // Check if '>' is at the beginning
        if (redirPos == 0) {
            cout << "Redirection error: Misplaced '>' symbol." << endl;
            return;
        }

        // Check if anything exists after '>'
        if (redirPos + 1 >= command.length()) {
            cout << "Redirection error: No file specified after '>'." << endl;
            return;
        }

        cmdPart = command.substr(0, redirPos);
        redirectFile = command.substr(redirPos + 1);

        // Remove trailing spaces from command
        cmdPart = cmdPart.substr(0, cmdPart.find_last_not_of(" \t") + 1);

        // Remove leading spaces from redirectFile
        size_t firstNonSpace = redirectFile.find_first_not_of(" \t");
        if (firstNonSpace == string::npos) {
            cout << "Redirection error: No file specified after '>'." << endl;
            return;
        }
        redirectFile = redirectFile.substr(firstNonSpace);

        // Make sure the redirection target isn't another '>'
        if (redirectFile[0] == '>') {
            cout << "Redirection error: Invalid file name after '>'." << endl;
            return;
        }
    }


    // Organize command & parameter by splitting input (Easier & More Consistent)
    vector<string> args;
    size_t pos = 0, found;
    while ((found = cmdPart.find(' ', pos)) != string::npos) {
        args.push_back(cmdPart.substr(pos, found - pos));
        pos = found + 1;
    }
    args.push_back(cmdPart.substr(pos));

    if (args[0] == "exit") {
        cout << "Jayden Minor\nAggie Shell Project" << endl;
        exit(0);
    } else if (args[0] == "cd") {
        if (args.size() != 2) {
            cout << "cd: Requires exactly one argument" << endl;
        } else if (chdir(args[1].c_str()) != 0) {
            perror("cd");
        }
    } else if (args[0] == "path") {
        searchPaths.clear();
        for (size_t i = 1; i < args.size(); i++) {
            searchPaths.push_back(args[i]);
        }
    } else {
        executeCommand(args, redirectFile);
    }
}


int main() {
    // Initialize search paths
    searchPaths.push_back("/bin");
    searchPaths.push_back("/usr/bin");

    string cmd;
    while (true) {
        cout << getPrompt();
        getline(cin, cmd);
        processCommand(cmd);
    }
    return 0;
}
