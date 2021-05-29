#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


//Samuel Evans 4/12/2021
//multiplies large strings by 2 using semaphores

sem_t semaphore;
int cnt = 0; //global shared variable
void *threadProd(void *param); //threads call this function.
int main(int argc, char *argv[])
{
    int niters;
    pthread_t tid1, tid2, tid3, tid4, tid5; //thread ids
    sem_init(&semaphore, 0, 1);
    if (argc != 2){
        printf("Usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);
    pthread_create(&tid1, NULL, threadProd,  &niters);
    pthread_create(&tid2, NULL, threadProd,  &niters);
    pthread_join(tid1, NULL); //wait for thread to finish
    pthread_join(tid2, NULL);
    sem_destroy(&semaphore);
    //check answer:
    if(cnt != (2 * niters))
        printf("Incorrect answer, cnt = %d\n", cnt);
    else
        printf("Correct answer, cnt = %d\n", cnt);
    exit(0);
    
}

//Thread routine
void *threadProd(void *vargp)
{
   
    
    int upper = *((int *) vargp);
    for (int i = 0; i < upper; i++){
	sem_wait(&semaphore);
        cnt ++;
	sem_post(&semaphore);
}
    return NULL;
}




