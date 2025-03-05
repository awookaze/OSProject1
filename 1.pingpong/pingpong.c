#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2]; // p1 for parent -> child, p2 for child -> parent
    char buf;

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) { // Child process
        close(p1[1]); // Close unused write end
        close(p2[0]); // Close unused read end

        read(p1[0], &buf, 1); // Read from parent
        printf("%d: received ping\n", getpid());

        write(p2[1], &buf, 1); // Send response to parent

        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else { // Parent process
        close(p1[0]); // Close unused read end
        close(p2[1]); // Close unused write end

        write(p1[1], "x", 1); // Send byte to child
        read(p2[0], &buf, 1); // Receive response from child

        printf("%d: received pong\n", getpid());

        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}
