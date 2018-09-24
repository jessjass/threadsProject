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

// void *compare(void * arguments){
//   printf("EXECUTING THREAD!\n");
//   thread_arg * arg = (thread_arg *) arguments;
//   //   int k = 0;
//   //unsigned int numRounds = getLog2(count);
//   //for (k = 0; k < numRounds; k++){
//   // Compare nums and set max val to ret val
//   if (arg->num1 >= arg->num2)
//     arg->return_value = arg->num1;
//   else
//     arg->return_value = arg->num2;
//   
//   return;
// }

// typedef struct{
//   float num1;
//   float num2;
//   float return_value;
// } thread_arg;

typedef struct{
    float * array;
  }thread_arg;

int count = 0;
// float * nums = malloc(3200);
// float nums[3220];

unsigned int getLog2 (unsigned int n){
    unsigned int logVal = 0;
    --n;
    while (n > 0) {
        ++logVal;
        n >>= 1;
    }
    return logVal;
}

void *compare(void * arguments){
  
  thread_arg * arg = (thread_arg *) arguments;
  int i =0;
  do {
    printf("EXECUTING THREAD!\n");
    printf("NUM IN ARRAY: %f\n", arg->array[i]);
    if(arg->array[i] == 4.0){
      return;
    }
    i++;
  } while (1);
}

int main(){
  
  /////////  --------------------------------------------------  //////////////
  
  char buffer[5];
  float * nums = malloc(3200);
  count = 0;
  
  printf("Enter floats: \n");
  
  // Read in stream of numbers
  while (1){
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      fprintf(stderr, "Error in fgets()\n");
      exit(EXIT_FAILURE);
    }
    
    if (sscanf(buffer, "%f", (nums + (count))) == 1) {
      printf("You entered: %f\n", nums[count]);
      count++;
    } else {
      break;
    }
  }
  
  printf("Count: %d\n", count);

  /////////  -----------------------------------------------  //////////////
  
  // Declare necessary variables
  int numOfThreads = count/2;
  pthread_t threadsArr[count/2];
  pthread_attr_t attr;
  
  thread_arg argument;
  argument.array = nums;
  
  printf("numOfThreads: %d\n", numOfThreads);
  
    int i = 0;
    int j = 0;
    pthread_attr_init(&attr);
    for (i=0; i < count-1; i+=2){
      printf("in for loop\n");
      pthread_create(&threadsArr[j], &attr, compare, &argument); //&argsArr[j]);
      j++;
    }
  
  int l;
  for (l=0; l < count/2; l++){
    pthread_join(threadsArr[l], NULL);
  }

  
  //free(nums);
  return 0;
}


