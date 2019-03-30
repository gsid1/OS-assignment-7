#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t mutex,write;
int data = 0,r = 0;

void *reader(void *arg)
{
  int f;
  f = ((int)arg);
  sem_wait(&mutex);
  r = r + 1;
  if(r==1)
   sem_wait(&write);
  sem_post(&mutex);
  printf("Reader %d Reads %d\n",f,data);
  sleep(1);
  sem_wait(&mutex);
  r = r - 1;
  if(r==0)
   sem_post(&write);
  sem_post(&mutex);
}

void *writer(void *arg)
{
  int f;
  f = ((int) arg);
  sem_wait(&write);
  data++;
  printf("Writer %d writes %d\n",f,data);
  sleep(1);
  sem_post(&write);
}

int main()
{
  int i,b; 
  pthread_t read[5],writ[5];
  sem_init(&mutex,0,1);
  sem_init(&write,0,1);
  for(i=0;i<=2;i++)
  {
    pthread_create(&writ[i],NULL,writer,(void *)i);
    pthread_create(&read[i],NULL,reader,(void *)i);
  }
  for(i=0;i<=2;i++)
  {
    pthread_join(writ[i],NULL);
    pthread_join(read[i],NULL);
  }
  return 0;
}