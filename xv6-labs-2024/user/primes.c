#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 280
#define FIRST_PRIME 2

// Function to generate numbers from FIRST_PRIME to MAX
int original() {
    int ori_pipe[2];
    
    pipe(ori_pipe);

    if (!fork()) { // Child process generates numbers
        for (int i = FIRST_PRIME; i < MAX; i++) {
            write(ori_pipe[1], &i, sizeof(int));
        }
        close(ori_pipe[1]); // Close write-end after writing all numbers

        exit(0);
    }

    close(ori_pipe[1]); // Parent closes write-end

    return ori_pipe[0]; // Return read-end of the pipe
}

// Function to filter out multiples of 'prime'
int prime_filter(int in_fd, int prime) {
    int num;
    int new_pipe[2];

    pipe(new_pipe);

    if (!fork()) { // Child process filters numbers
        while (read(in_fd, &num, sizeof(int))) {
            if (num % prime) { // If num is not divisible by prime, pass it
                write(new_pipe[1], &num, sizeof(int));
            }
        }
        close(in_fd);      // Close previous input pipe
        close(new_pipe[1]); // Close write-end after processing
        
        exit(0);
    }

    close(in_fd);      // Parent closes input pipe
    close(new_pipe[1]); // Parent closes write-end

    return new_pipe[0]; // Return read-end for next filter
}

int main(int argc, char *argv[]) {
    int prime;
    
    int in = original();
    while (read(in, &prime, sizeof(int))) {
        printf("prime %d\n", prime);
        in = prime_filter(in, prime);
    }

    exit(0);
}

