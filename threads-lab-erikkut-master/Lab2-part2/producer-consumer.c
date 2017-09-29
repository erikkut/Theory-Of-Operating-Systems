#include <stdio.h>
#include <pthread.h>
#include "queues.h" //brings an editable queue into the program
#define MAX 100

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;
int sum = 0;//used to print out the sum of every 10 elements

void *producer(void *ptr){//produces 100 elements (x4 since four are created)
  int i;
  for(i = 0; i < MAX; i++){
    pthread_mutex_lock(&the_mutex);  //get exclusive access to buffer
    while(buffer != 0) pthread_cond_wait(&condp, &the_mutex);
    buffer = 1;
    
    enqueue(i);//enqueue producer's index (0...99 each one)
    
    pthread_cond_signal(&condc);     //wake consumer
    pthread_mutex_unlock(&the_mutex);//release access to buffer
  }
  pthread_exit(0);
}

void *consumer(void *ptr){//consumes  all elements created by producers
  int i;
  int tenSum = 0;
  for(i = 0; i < (MAX*4); i++){
    pthread_mutex_lock(&the_mutex);  //exclusive access
    while(buffer == 0) pthread_cond_wait(&condc, &the_mutex);
    buffer = 0;

    sum += dequeue();//dequeue from producers and add to sum
    if(i == 1)//decrements first sum ONLY by 1 due to index starting at 0
      sum-=1;
    if((i+1) % 10 == 0){//if sum reaches production limit print and reset
      printf("Sum: %d\n", sum);
      sum = 0;
    }

    pthread_cond_signal(&condp);     //wake all 4 producers
    pthread_mutex_unlock(&the_mutex);//release access to buffer
  }
  pthread_exit(0);
}

int main(int argc, char **argv){
  printf("Starting program\n\n");

  //initializes 4 producers and 1consumer
  pthread_t pro1, pro2, pro3, pro4, con;

  //initialize the mutex, cond, and threads
  pthread_mutex_init(&the_mutex, 0);
  pthread_cond_init(&condc, 0);
  pthread_cond_init(&condp, 0);
  pthread_create(&con, 0, consumer, 0);
  pthread_create(&pro1, 0, producer, 0);
  pthread_create(&pro2, 0, producer, 0);
  pthread_create(&pro3, 0, producer, 0);
  pthread_create(&pro4, 0, producer, 0);

  //join the processes
  pthread_join(pro1, 0);
  pthread_join(pro2, 0);
  pthread_join(pro3, 0);
  pthread_join(pro4, 0);
  pthread_join(con, 0);

  //clean
  pthread_cond_destroy(&condc);
  pthread_cond_destroy(&condp);
  pthread_mutex_destroy(&the_mutex);  
  
  printf("\nProgram Done\n");
}
