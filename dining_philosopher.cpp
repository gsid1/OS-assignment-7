#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
  
#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (phl + 4) % N 
#define RIGHT (phl + 1) % N 
  
int state[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 
  
sem_t mutex; 
sem_t S[N]; 
  
void test(int phl) 
{ 
    if (state[phl] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) { 
        state[phl] = EATING; 
        sleep(2); 
        printf("Philosopher %d takes fork %d and %d\n", phl + 1, LEFT + 1, phl + 1); 
        printf("Philosopher %d is Eating\n", phl + 1); 
        sem_post(&S[phl]); 
    } 
} 

void fork_1(int phl) 
{ 
    sem_wait(&mutex); 
    state[phl] = HUNGRY; 
    printf("Philosopher %d is Hungry\n", phl + 1);  
    test(phl); 
    sem_post(&mutex); 
    sem_wait(&S[phl]); 
    sleep(1); 
} 
  
void fork_2(int phl) 
{ 
    sem_wait(&mutex); 
    state[phl] = THINKING; 
    printf("Philosopher %d putting fork %d and %d down\n", phl + 1, LEFT + 1, phl + 1); 
    printf("Philosopher %d is thinking\n", phl + 1); 
    test(LEFT); 
    test(RIGHT); 
    sem_post(&mutex); 
} 
  
void* philospher(void* num) 
{ 
    while (1) { 
        int* i = num; 
        sleep(1); 
        fork_1(*i); 
        sleep(0); 
        fork_2(*i); 
    } 
} 
  
int main() 
{ 
    int i; 
    pthread_t thread_id[N]; 
    sem_init(&mutex, 0, 1); 
    for (i = 0; i < N; i++) 
        sem_init(&S[i], 0, 0); 

    for (i = 0; i < N; i++) 
    {

        pthread_create(&thread_id[i], NULL, philospher, &phil[i]); 
        printf("Philosopher %d is thinking\n", i + 1); 
    
    } 
    for (i = 0; i < N; i++) 
        pthread_join(thread_id[i], NULL); 
} 
