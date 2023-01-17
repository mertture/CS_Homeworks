#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

    printf("I’m SHELL process, with PID: %d - Main command is: man rm | grep -w -A 1 \"\\-v\" > output.txt\n", (int)getpid());


    int fd[2];
    if (pipe(fd) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    pid_t cpids[2];
    cpids[0] = fork();

    if (cpids[0] < 0) {
        // fork just failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (cpids[0] == 0) {
        
        printf("I’m MAN process, with PID: %d - My command is: man rm\n", (int)getpid());

        cpids[1] = fork();

        if (cpids[1] < 0) {
            // fork just failed
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        else if (cpids[1] == 0) {
            printf("I’m GREP process, with PID: %d - My command is: grep -w -A 1 \"\\-v\" > output.txt \n", (int)getpid());
            
            close(fd[1]);
            
            int new_file = open("./output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
            
            if (dup2(new_file, STDOUT_FILENO) < 0) {
                perror("dup2");
                _exit(1);
            }

            if (dup2(fd[0], STDIN_FILENO) < 0) {
                perror("dup2");
                _exit(1);
            }
           

            char *myargs2[6];
            myargs2[0] = strdup("grep"); 
            myargs2[1] = strdup("-w"); 
            myargs2[2] = strdup("-A");   
            myargs2[3] = strdup("1");
            myargs2[4] = strdup("\\-v");  
            myargs2[5] = NULL;        
            execvp(myargs2[0], myargs2);
        }
        else {
            close(fd[0]);        
            
            if (dup2(fd[1], STDOUT_FILENO) < 0) {
                perror("dup2");
                _exit(1);
            }

            char *myargs[3];
            myargs[0] = strdup("man"); 
            myargs[1] = strdup("rm");  
            myargs[2] = NULL;
            execvp(myargs[0], myargs);
        }
    }
    else {
        waitpid(cpids[1], NULL, 0);
        printf("I’m SHELL process, with PID:%d - execution is completed, you can find the results in output.txt\n", (int)getpid());
    }

    return 0;
}
