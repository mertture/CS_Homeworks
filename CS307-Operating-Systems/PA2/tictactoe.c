#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>


pthread_mutex_t lock;
int N;

char ** sharedBoard;

char turn = 'x';
int gameFinished = 0;
int winX = 0;
int winO = 0;


void createBoard(int N) {

    int i,j;

    sharedBoard = (char **)malloc(sizeof(char*) * N);

    for (i = 0; i < N; i++) {
        sharedBoard[i] = (char *) malloc(N);
        for (j = 0; j < N; j++) {
            sharedBoard[i][j] = '.';
        }
    }
    printf("Board Size: %dx%d\n", N, N);

}



int putSign(char sign) {

    int x = rand() % N;
    int y = rand() % N;


    while (sharedBoard[x][y] != '.') {
        x = rand() % N;
        y = rand() % N;
    }

    sharedBoard[x][y] = sign;
    printf("Player %c played on: (%d,%d)\n", sign, x, y);
    int i, j;
    // rows
    for (i = 0; i < N; i++) {
        int counter = 0;
        for (j = 0; j < N; j++) {
            if (sharedBoard[i][j] == sign) {
                counter++;
            }
        }
        if (counter == N) {
            gameFinished = 1;
            return 1;
        }
    }
    // columns
    for (i = 0; i < N; i++) {
        int counter = 0;
        for (j = 0; j < N; j++) {
            if (sharedBoard[j][i] == sign) {
                counter++;
            }
        }
        if (counter == N) {
            gameFinished = 1;
            return 1;
        }
    }
    // left to right diag
    int counter = 0;
    for (i = 0, j = 0; i < N && j < N; i++, j++) {
        if (sharedBoard[i][j] == sign) {
                counter++;
        }
    }
    if (counter == N) {
        gameFinished = 1;
        return 1;
    }

// right to left diag
    counter = 0;
    for (i = 0, j = N-1; i < N && j >= 0; i++, j--) {
        if (sharedBoard[i][j] == sign) {
                counter++;
        }
    }
    if (counter == N) {
        gameFinished = 1;
        return 1;
    }
    counter = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (sharedBoard[i][j] != '.') {
                counter++;
            }
        }
    }

    if (counter == N*N) {
        gameFinished = 1;
        return 0;
    }


    return 0;
}






void *turnX(void * nl) {
    // loop is using CPU
    while(1) {
        // waiting for O to unlock
        pthread_mutex_lock(&lock);
        if (gameFinished) {    
           pthread_mutex_unlock(&lock);
           pthread_exit(NULL);
        }
        // to prevent deadlock and break if turn is on O
        else if (turn == 'o') {
            pthread_mutex_unlock(&lock);
        }
        else {
            winX = putSign('x');
            // X win or board is full
            turn = 'o';
            if (winX == 1 || gameFinished) {
                
                gameFinished = 1;
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL);
                break;
            }
            else {
                //turn = 'O';
                pthread_mutex_unlock(&lock);
            }
        }
    }
}





void *turnO(void * nl) {
    // loop is using CPU and waits for X to unlock
    while(1) {
        pthread_mutex_lock(&lock);
        if (gameFinished) {    
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL);
        }
        // to prevent deadlock and break if turn is on X
        else if (turn == 'x') {
            pthread_mutex_unlock(&lock);
        }
        else {
            winO = putSign('o');
            // O win or board is full
            turn = 'x';
            if (winO == 1 || gameFinished) {
                // if board is full
                gameFinished = 1;
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL);
                break;
            }
            else {
                pthread_mutex_unlock(&lock);
            }
        }
    }
}







int main(int argc, char *argv[]) {
    
    srand(time(NULL));
    N = atoi(argv[1]);
   

    createBoard(N);
    
    pthread_t px, po;
    int playXth = pthread_create(&px, NULL, turnX, NULL);
    if (playXth != 0) {
        return 0;
    }
    int playOth = pthread_create(&po, NULL, turnO, NULL);
    if (playOth != 0) {
        return 0;
    }


    pthread_join(px, NULL);
    pthread_join(po, NULL);


    printf("Game end\n");
    if (winX) {
        printf("Winner is X\n");
    }
    else if (winO) {
        printf("Winner is O\n");
    }
    else {
        printf("It is a tie\n");
    }
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (sharedBoard[i][j] != '.') {
                printf("[%c]", sharedBoard[i][j]);
            }
            else {
                printf("[ ]");
            }
        }
        printf("\n");
    }
    pthread_mutex_destroy(&lock);
    
    return 0;
}