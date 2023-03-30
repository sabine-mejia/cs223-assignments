#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




int main() {

  int list_size, val;
  int* int_arr;
  pid_t pid;


  printf("Enter a value to search: \n");
  scanf("%d", &val);
  
  FILE* fp = fopen("data.bin", "rb");
  if(fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  //read first value of the binary file (this gives you the size of the list)
  fread(&list_size, sizeof(int), 1, fp);
  printf("size: %d\n", list_size);

  int_arr = malloc(sizeof(int) * list_size);
  fread(int_arr, sizeof(int), list_size, fp);

  fclose(fp); 
  
  pid = fork();

  //child process: search second half of list
  if(pid == 0) {
    for(int i = list_size/2; i < list_size; i ++) {
      if(int_arr[i] == val) {
        printf("Child found %d at index %d\n", val, i);
      }
    }
  }
  else {
    for(int i = 0; i < list_size/2; i++) {
      if(int_arr[i] == val) {
        printf("Parent found %d at index %d\n", val, i);
      }
    }
  }

  free(int_arr);
  return 0;
}
