#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define BUFF 10
#define FULL 0
#define EMPTY 0
char buffer[BUFF];
int nextIn = 0;
int nextOut = 0;

sem_t empty_sem; //producer semaphore
sem_t full_sem; //consumer semaphore

void produce(char data)
{
  int value;
  sem_wait(&empty_sem); //get the mutex to fill the buffer

  buffer[nextIn] = data;
  nextIn = (nextIn + 1) % BUFF;
  printf("Producing %c nextIn %d Ascii=%d\n",data,nextIn,data);
  if(nextIn==FULL)
    {
      sem_post(&full_sem);
      sleep(1);
    }
  sem_post(&empty_sem);

}

void * Producer(void* arg)
{
  int i;
  for(i = 0; i < 10; i++)
    {
      produce((char)('A'+ i % 26));
    }
}

void consume()
{
  int data;

  sem_wait(&full_sem); // gain the mutex to consume from buffer

  data = buffer[nextOut];
  nextOut = (nextOut + 1) % BUFF;
  printf("\tConsuming %c nextOut %d Ascii=%d\n",data,nextOut,data);
  if(nextOut==EMPTY) //its empty
    {
      sleep(1);
    }

  sem_post(&full_sem);
}

void * Consumer(void* arg)
{
  int i;
  for(i = 0; i < 10; i++)
    {
      consume();
    }
}

int main()
{
  pthread_t producer_thread,consumer_thread;
  //initialize the semaphores

  sem_init(&empty_sem,0,1);
  sem_init(&full_sem,0,0);

  //creating producer and consumer threads

  if(pthread_create(&producer_thread, NULL,Producer, NULL))
    {
      //printf("\n ERROR creating thread 1");
      exit(1);
    }

  if(pthread_create(&consumer_thread, NULL,Consumer, NULL))
    {
      //printf("\n ERROR creating thread 2");
      exit(1);
    }

  if(pthread_join(producer_thread, NULL)) /* wait for the producer to finish */
    {
      //printf("\n ERROR joining thread");
      exit(1);
    }

  if(pthread_join(consumer_thread, NULL)) /* wait for consumer to finish */
    {
      //printf("\n ERROR joining thread");
      exit(1);
    }

  sem_destroy(&empty_sem);
  sem_destroy(&full_sem);

  //exit the main thread

  pthread_exit(NULL);
  return 1;
}