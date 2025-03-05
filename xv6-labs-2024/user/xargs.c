#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    char line[512];
    char *args[MAXARG];
    int initial_args = argc - 1;

    while (1) {
        int pos = 0;
        char c;
        // Read a line from stdin
        while (1) {
            int n = read(0, &c, 1);
            if (n < 0) {
                fprintf(2, "xargs: read error\n");
                exit(1);
            }
            if (n == 0) { // EOF
                if (pos == 0) exit(0);
                break;
            }
            if (c == '\n') break;
            if (pos < sizeof(line) - 1) {
                line[pos++] = c;
            }
        }
        line[pos] = '\0'; // Null-terminate the line

        // Split the line into arguments
        int num_args = 0;
        char *p = line;
        while (*p != '\0') {
            // Skip whitespace
            while (*p == ' ' || *p == '\t') {
                *p++ = '\0';
            }
            if (*p == '\0') break;
            args[num_args++] = p;
            // Move to end of token
            while (*p != '\0' && *p != ' ' && *p != '\t') {
                p++;
            }
            if (num_args >= MAXARG - initial_args) break; // Avoid overflow
        }

        // Check if total arguments exceed MAXARG
        if (initial_args + num_args >= MAXARG) {
            fprintf(2, "xargs: too many arguments\n");
            exit(1);
        }

        // Prepare exec arguments
        char *exec_argv[MAXARG];
        for (int i = 0; i < initial_args; i++) {
            exec_argv[i] = argv[i + 1];
        }
        for (int i = 0; i < num_args; i++) {
            exec_argv[initial_args + i] = args[i];
        }
        exec_argv[initial_args + num_args] = 0;

        // Execute command
        int pid = fork();
        if (pid < 0) {
            fprintf(2, "xargs: fork failed\n");
            exit(1);
        } else if (pid == 0) {
            // Child process
            exec(exec_argv[0], exec_argv);
            fprintf(2, "xargs: exec %s failed\n", exec_argv[0]);
            exit(1);
        } else {
            // Parent waits for child
            wait(0);
        }
    }
    exit(0);
}
