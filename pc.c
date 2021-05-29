/*
 * Jimmie Gann, Sam Evans
 * 05/04
 * Producer-Consumer Problem
 * CSC 345 002
 */

//Neccessary libraries
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

//Line 15-55 was given
typedef struct {
char *buffer; /* Buffer array */
int n;     /* Maximum number of slots */
int front; /*  first item */
int rear;  /* last item */
sem_t mutex; /* sempahore to protect accesses to buf */
sem_t slots; /* sempaphore to count available slots */
sem_t items; /* semaphore to count available items */
} sbuf_t;

void sbuf_init(sbuf_t *sb, int n);  /* Initialize the data structure sbuf_t members...*/
void sbuf_clean(sbuf_t *sp);        /* Free memory allocated for the buffer */
char produceNextLetter(int i);      /*produce the next letter, starting with 'a'*/

void *producer(void *); /* called by producer thread */
void *consumer(void *); /* called by consumer thread */


int main(int argc, char* argv[]){
    
    
    if(argc != 2){
        printf("Usage: %s n where n is the buffer size\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    sbuf_t sb;
    sbuf_init(&sb, n);

    pthread_t tprod;
    pthread_t tcons;
    pthread_create(&tprod, NULL, producer, &sb);
    pthread_create(&tcons, NULL, consumer, &sb);

    pthread_join(tprod, NULL);
    pthread_join(tcons, NULL);
    sbuf_clean(&sb);
    return 0;
}

// Here all of the initial values are given
void sbuf_init(sbuf_t *sb, int n){
    sb->buffer = (char *)malloc(sizeof(char) *n);
    
    sb->n = n;
    sb->front = 0;
    sb->rear = 0;
    //Initializes the semaphores using a refrence to the global variables
    sem_init(&(sb->mutex), 0 ,1);
    sem_init(&(sb->slots), 0 ,1);
    sem_init(&(sb->items), 0 ,1);
    
}

/* 
To find the next letter we created an array of the alphabet then kept track of a global variable that was constantly being 
incremented and used that to know which indice to grab
*/
char produceNextLetter(int i){
   char alph[26] = {'a','b', 'c', 'd', 'e', 'f' ,'g', 'h', 'i' , 'j' , 'k' ,'l', 'm' ,'n' ,'o', 'p', 'q', 'r','s','t','u','v','w','x','y','z'};
   
   return alph[i];
   i++;

	
}

// Where the producer will add letters to the buffer
void *producer(void *a){
    
    // Creating a object of our structure
    sbuf_t *s = (sbuf_t*)a;

    // All the variables that are used in our function
    char temp;
    int x = 0;
    int y = 0;
    int counter = 1;
    
    // This loop will run until the user stops the program
    while(1){
        
        sem_wait(&(s->slots));
        sem_wait(&(s->mutex));
        
        // Using the indice "x" we save the next letter into a temp value
        // We then set the current front value in the buffer to temp and incriment temp as well as both of out index trackers
        temp = produceNextLetter(x);
        s->buffer[s->front] = temp;
        s->front++;
        x++;
        y++;
 
        // If our x is more than 26 it will be at the end of our array, we reset the x value in this case so that it will go back to a
        if(x == 26) {
            x = 0;
        }

        // Shows the value that was produced then runs the sem_post commands
        printf("Producer: new char = %c\n", temp);

        sem_post(&(s->mutex));
        sem_post(&(s->items));

        // This is how we make sure the buffer never gets bigger than size n
        // It keeps track of how many times we have reached the end of the buffer (counter variable) and divides the total number of times a ketter has been produced
        // If this value is greater than the user-defined n value, it knows to reset it to 0 
        if(y/counter >= (s->n)){
            s->front = 0;
            counter++;
        }

    }
}

void *consumer(void *a){
    
    // Creating a object of our structure
    sbuf_t *s = (sbuf_t*)a;

    // All the variables that are used in our function   
    char temp;
    int y = 0;
    int counter = 1;

    // This loop will run until the user stops the program
    while(1){

        sem_wait(&(s->items));
        sem_wait(&(s->mutex));

        // For the consumer our temp value will be whatever the value at the rear index is
        // It will find this value then increment the rear index
        temp = s->buffer[s->rear];
        s->rear++;

        sem_post(&(s->mutex));
        sem_post(&(s->slots));

        // Prints out the character that was just comsumed then incriments ys
        printf("Consumer: char = %c\n", temp);
        y++;

        // This is how we make sure the buffer never gets bigger than size n
        // It keeps track of how many times we have reached the end of the buffer (counter variable) and divides the total number of times a ketter has been produced
        // If this value is greater than the user-defined n value, it knows to reset it to 0   
        if(y/counter >= (s->n)){
            s->rear = 0;
            counter++;
        }

    }
}

void sbuf_clean(sbuf_t *sb){
    free(sb->buffer);
}

