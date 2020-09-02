#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#define NTHRS 5

sem_t sem;
pthread_mutex_t mtx;
int S = 0;

void barrier_point()
{
    pthread_mutex_lock(&mtx);
    S++;
	pthread_mutex_unlock(&mtx);

	if(S<NTHRS) {
		if(sem_wait(&sem)) {
			perror(NULL);
			return errno;
		}
    }
    else for(int i = 0; i<NTHRS - 1; ++i)
		sem_post(&sem);
}

void *tfun(void *v) {
	int *tid = (int *)v;
	printf("%d reached the barrier \n",*tid);
	barrier_point(); // functia semafor 
	printf("%d passed the barrier \n", *tid);
	return NULL;
}

int main()
{
	pthread_t thr[100];
	int i;
	if(sem_init(&sem, 0, S)) { // al 3-lea parametru - 0 - wait, 1 - 
		perror(NULL);
		return errno;
	}

	if(pthread_mutex_init(&mtx,NULL)) {
		perror(NULL);
		return errno;
	}

	for(i=0; i < NTHRS; i++) {
        int *k = (int *)malloc(sizeof(int*));
        *k = i;
		if(pthread_create(&thr[i],NULL,tfun,k)) {
			perror(NULL);
			return errno;
		}
	}

		for(i=0;i<NTHRS; i++) {
			if(pthread_join(thr[i], NULL)) {
				perror(NULL);
				return errno;
			}
		}
	return 0;
}

