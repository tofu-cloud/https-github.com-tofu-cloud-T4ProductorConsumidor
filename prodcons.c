#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM 5
#define PROD 10

int buffer[TAM];

int front = 0;
int rear = 0;

sem_t empty;
sem_t full;

sem_t mutexP;
sem_t mutexC;

void *productor(void *arg)
{
    int id = *(int *)arg;

    for(int i = 1; i <= PROD; i++)
    {
        int dato = id * 100 + i;
        sem_wait(&empty);
        sem_wait(&mutexP);
        buffer[rear] = dato;
        printf("Productor %d produce %d en [%d]\n", id, dato, rear);
        rear = (rear + 1) % TAM;
        sem_post(&mutexP);
        sem_post(&full);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg)
{
    int id = *(int *)arg;
    for(int i = 1; i <= PROD; i++)
    {
        int dato;
        sem_wait(&full);
        sem_wait(&mutexC);
        dato = buffer[front];
        printf("\tConsumidor %d consume %d de [%d]\n", id, dato, front);
        front = (front + 1) % TAM;
        sem_post(&mutexC);
        sem_post(&empty);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t prod1, prod2;
    pthread_t cons1, cons2;

    int p1 = 1;
    int p2 = 2;

    int c1 = 1;
    int c2 = 2;

    sem_init(&empty, 0, TAM);
    sem_init(&full, 0, 0);

    sem_init(&mutexP, 0, 1);
    sem_init(&mutexC, 0, 1);

    pthread_create(&prod1, NULL, productor, &p1);
    pthread_create(&prod2, NULL, productor, &p2);
    pthread_create(&cons1, NULL, consumidor, &c1);
    pthread_create(&cons2, NULL, consumidor, &c2);

    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutexP);
    sem_destroy(&mutexC);

    return 0;
}
