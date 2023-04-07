#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct t_arg {
  int *array; // pointer to shared array
  long length; // num elements in array
  long numthreads; // total number of threads
  long id; // logical thread id
  int value; // 
  int returnval;
};

void *search(void* args) {
  //cast to a struct t_arg from void*
  struct t_arg * myargs = (struct t_arg *) args;

  //extract all variables from struct
  long myid =  myargs->id;
  long length = myargs->length; //equal to list_size
  long nthreads = myargs->numthreads;
  int * ap = myargs->array; //pointer to array in main points to int_arr
  int val = myargs->value;

  //to search ...
  long chunk = length/nthreads;
  long start = myid * chunk;
  long end  = start + chunk;
  if(myid == nthreads-1) {
    end = length;
  }

  for (int i = start; i < end; i++) {
    if(ap[i] == val) {
      printf("Thread %ld found %d at index %d\n", myid, val, i);
      myargs->returnval = i;
    }
    else{
      myargs->returnval = -1;
    }
  }

  return NULL;
}



int main(int argc, char** argv) {
  if(argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  int num_threads = strtol(argv[1], NULL, 10);
  int list_size, val;
  int* int_arr;

  printf("Enter a value to search: \n");
  scanf("%d", &val);
  
  FILE* fp = fopen("data.bin", "rb");
  if(fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  //read first value of the binary file (this gives you the size of the list)
  fread(&list_size, sizeof(int), 1, fp);

  //malloc an array of integers to hold the list
  int_arr = malloc(sizeof(int) * list_size);
  fread(int_arr, sizeof(int), list_size, fp); //read all integers in the list into the array

  fclose(fp); 


  //allocate space for thread structs and identifiers
  pthread_t *thread_array = malloc(num_threads * sizeof(pthread_t)); //initialize an array of pointers to threads
  struct t_arg *thread_args = malloc(num_threads * sizeof(struct t_arg)); //initialize an array of pointers to structs

  //initialize structs in thread_args
  for(int i = 0; i < num_threads; i++) {
    thread_args[i].array = int_arr;
    thread_args[i].length = list_size;
    thread_args[i].numthreads = num_threads;
    thread_args[i].id = i;
    thread_args[i].value = val;
    thread_args[i].returnval = -1;
  }

  //initialize threads in thread_array
  for (int i = 0; i < num_threads; i++) {
    pthread_create(&thread_array[i], NULL, search, &thread_args[i]);
  }   

  int count = 0;
  for (int i = 0; i < num_threads; i++) {
    pthread_join(thread_array[i], NULL);
    if(thread_args[i].returnval == -1) {
    }
    else {
      count ++;
    }
  }
  if(count == num_threads) {
      printf("Not found!\n");
  }

  free(int_arr);
  return 0;
}
