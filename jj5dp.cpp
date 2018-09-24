/*
 * 
 * Jessica Jassal
 * Operatig Systems Homework 3
 * Spring 2018
 * 
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

///////////////////// CREATE BARRIER ///////////////////////

class Barrier {
  private:
    int value, init;
    sem_t mutex;
    sem_t waitq;
    sem_t throttle;
  public:
    Barrier(int in);
    void wait(int newVal);
};

// BARRIER CONSTRUCTOR
Barrier::Barrier(int in){
  init = in;
  value = init;
  sem_init(&mutex, 0, 1);
  sem_init(&waitq, 0, 0);
  sem_init(&throttle, 0, 0);
}

// WAIT() METHOD
void Barrier::wait(int newVal){
  sem_wait(&mutex);
  value--;
  if(value != 0){ // NOT LAST THREAD
    sem_post(&mutex);
    sem_wait(&waitq);
    sem_post(&throttle);
  } else { // LAST THREAD
    int i = 0;
    for (i = 0; i < init-1; i++){ // RELEASE WAITING THREADS
      sem_post(&waitq);
      sem_wait(&throttle);
    }
    init = newVal;
    value = init;
    sem_post(&mutex);
  }
}

///////////////////// GLOBAL VARS ///////////////////////

typedef struct{
    float * array;
    pthread_t tid;
    int first;
    int second;
    int numThreads;
    Barrier * bar;
}thread_arg;

int count = 0;
unsigned int numOfRounds = 0;

///////////////////// HELPER FUNCTION FOR ROUND NUMS ///////////////////////

unsigned int getLog2 (unsigned int n){
    unsigned int logVal = 0;
    --n;
    while (n > 0) {
        ++logVal;
        n >>= 1;
    }
    return logVal;
}

///////////////////// THREAD FUNCTION ///////////////////////

void *compare(void * arguments){
  thread_arg * arg = (thread_arg *) arguments;
  int i = 1;
  for(i = 1; i <= numOfRounds; i++){
    
    // DECREASE NUMBER OF THREADS TO USE
    if((int)arg->tid < arg->numThreads){
      
      // COMPARING
      if (arg->array[arg->first] < arg->array[arg->second])
	arg->array[arg->first] = arg->array[arg->second];
      
      // BARRIER WAITING
      arg->bar->wait(arg->numThreads/2);
      
      // SETTING NEW ARG VALS
      arg->first = arg->first*2;
      arg->second = arg->second*2;
      arg->numThreads = arg->numThreads/2;
    }
  }
}

int main(){
  
  /////////  READ IN INPUT  //////////////
  
  char buffer[5];
  float * nums = new float[4*8193];
  count = 0;
  
  // REAN IN INPUTS
  while (1){
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      break;
    }
    
    if (sscanf(buffer, "%f", (nums + (count))) != -1) {
      count++;
    } else {
      break;
    }
  }

  /////////  SET UP AND CREATE THREADS  //////////////
  
  // DECLARE VARIABLES
  int numOfThreads = count/2;
  numOfRounds = getLog2(count);
  pthread_t threadsArr[count/2];
  thread_arg argsArr[numOfThreads];
  Barrier * b = new Barrier(numOfThreads);
  pthread_attr_t attr;
  
  // SET ARGUMENT VALS AND CREATE THREADS
  int i = 0;
  int j = 0;
  pthread_attr_init(&attr);
  for (i=0; i < count-1; i+=2){
    argsArr[j].array = nums;
    argsArr[j].tid = j;
    argsArr[j].first = i;
    argsArr[j].second = i+1;
    argsArr[j].numThreads = numOfThreads;
    argsArr[j].bar = b;
    
    // CREATE THREADS
    pthread_create(&threadsArr[j], &attr, compare, &argsArr[j]);
    j++;
  }
  
  // WAIT FOR THREADS TO TERMINATE
  int l;
  for (l=0; l < count/2; l++){
    pthread_join(threadsArr[l], NULL);
  }

  // PRINT MAX VALUE
  printf("%d \n", int(nums[0]));
  
  free(nums);
  free(b);
  return 0;
}


