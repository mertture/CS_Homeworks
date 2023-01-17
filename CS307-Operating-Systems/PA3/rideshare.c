#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>



int car_A = 0;
int car_B = 0;


pthread_mutex_t lock;


pthread_barrier_t carLimit;


sem_t semA;
sem_t semB;



void* putA() {
    int driver = 0;
    // lock in beginning
    pthread_mutex_lock(&lock);
    printf("Thread ID: %ld, Team: A, I am looking for a car\n", pthread_self());
    car_A++;

    // if 2 A - 2 B in a car
    if (car_A == 2 && car_B >= 2) {

        for (int i = 0; i < 2; i++) {
            // release 2 A to car
            sem_post(&semA);
        }
        for (int i = 0; i < 2; i++) {
            // release 2 B to car
            sem_post(&semB);
        }
        // when both came here with A 2 - B 2
        car_A = 0; car_B -= 2;
        // last joined is the driver
        driver = 1;
    }
    // if 4 A in a car
    else if (car_A == 4) {

        for (int i = 0; i < 4; i++) {
            // release 4 A to car
            sem_post(&semA);
        }

        car_A = 0;
        // last joined is the driver
        driver = 1;
    }
    else {
        // unlock it if car is not full because we will take it to sem_wait
        pthread_mutex_unlock(&lock);
    }
    // waits for others to open
    sem_wait(&semA);
    printf("Thread ID: %ld, Team: A, I have found a spot in a car\n", pthread_self());

    // waits until car is 4
    pthread_barrier_wait(&carLimit);

    // check the driver
    if (driver == 1) {
        printf("Thread ID: %ld, Team: A, I am the captain and driving the car\n", pthread_self());
        driver = 0;
        // Program comes this point if it goes into if or else if due to carLimit
        // mutex should be unlocked
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* putB() {
    int driver = 0;
    // lock in beginning
    pthread_mutex_lock(&lock);
    printf("Thread ID: %ld, Team: B, I am looking for a car\n", pthread_self());
    car_B++;

    if (car_B == 2 && car_A >= 2) {

        for (int i = 0; i < 2; i++) {
            sem_post(&semB);
        }
        for (int i = 0; i < 2; i++) {
            sem_post(&semA);
        }

        // when both came here with A 2 - B 2
        car_A -= 2; car_B = 0;
        // last joined is the driver
        driver = 1;
    }
    else if (car_B == 4) {

        for (int i = 0; i < 4; i++) {
            sem_post(&semB);
        }
        car_B = 0;
        // last joined is the driver
        driver = 1;
    }
    else {
        pthread_mutex_unlock(&lock);
    }

    sem_wait(&semB);
    printf("Thread ID: %ld, Team: B, I have found a spot in a car\n", pthread_self());

    pthread_barrier_wait(&carLimit);
    if (driver == 1) {
        printf("Thread ID: %ld, Team: B, I am the captain and driving the car\n", pthread_self());
        driver = 0;
        // Program comes this point if it goes into if or else if due to carLimit
        // mutex should be unlocked
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main (int argc, char* argv[]) {

    int teamA = atoi(argv[1]);
    int teamB = atoi(argv[2]);

    int all_fans = teamA + teamB;

    if (all_fans % 4 == 0 && teamA % 2 == 0 && teamB % 2 == 0) {

        sem_init(&semA, 0, 0);
        sem_init(&semB, 0, 0);

        pthread_barrier_init(&carLimit, NULL, 4);

        pthread_t* all_threads = (pthread_t*) malloc(sizeof(pthread_t) * all_fans);

        for (int i = 0; i < teamA; i++) {
            int ptA = pthread_create(&all_threads[i], NULL, putA, NULL);
        }

        for (int i = teamA; i < all_fans; i++) {
            int ptB = pthread_create(&all_threads[i], NULL, putB, NULL);
        }

        for (int i = 0; i < all_fans; i++) {
            pthread_join(all_threads[i], NULL);
        }
    }
    printf("The main terminates\n");
}