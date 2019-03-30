#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<semaphore.h>

#define N 5

sem_t mutex,customers,barbers;
int count=0;

void* barber(void *arg);
void* customer(void *arg);

int main(int argc,char *argv[])
{
	pthread_t id1,id2;
	int status=0;

	sem_init(&mutex,0,1);
	sem_init(&customers,0,0);
	sem_init(&barbers,0,1);

	status=pthread_create(&id1,NULL,barber,NULL);
	status=pthread_create(&id2,NULL,customer,NULL);


	pthread_join(id2,NULL);
	pthread_join(id1,NULL);

	exit(0);
}

void* barber(void *arg)
{
	while(1)
	{
		sem_wait(&customers);           
		sem_wait(&mutex);
		count--;
		printf("Barber cutting hair \tcount is:%d.\n",count);
		sem_post(&mutex);
		sem_post(&barbers);
		sleep(3);       
	}
}

void* customer(void *arg)
{
	while(1)
	{
		sem_wait(&mutex);
		if(count<N)
		{
			count++;
			printf("Customer Arrived \tcount is:%d\n",count);
			sem_post(&mutex);
			sem_post(&customers);
			sem_wait(&barbers);
		}
		else
			sem_post(&mutex);
		sleep(1);
	}
}