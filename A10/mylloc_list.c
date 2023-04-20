#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct chunk {
  int size; //size of memory allocated from sbrk
  int used; //4
  struct chunk *next; //8
};

struct chunk *flist = NULL; //this is a pointer to the free list called flist

/**
This is our implementation of the malloc function
*/
void *malloc(size_t size) {

  //requested size is 0, don't return anything
  if(size == 0) {
    return NULL;
  }

  struct chunk *next = flist;
  struct chunk *prev = NULL;

  //Go throught flist and find a block which is big enough
  while(next != NULL) {
    if(next->size >= size) {
      if(prev != NULL) {
        prev->next = next->next;
      }
      else {
        flist = next->next;
      }
      return (void *)(next + 1); //return a pointer to the start of the data in the chunk
    }
    else {
      prev = next;
      next = next -> next;
    }
  }

  //use sbrk to allocate new memory if none is allocated allready
  void *memory = sbrk(size + sizeof(struct chunk)); 

  if(memory == (void *) -1) {
    return NULL;
  }
  else {
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    cnk->used = size;
    return (void*)(cnk + 1); //return a pointer to memory
  }
  
}

/**
* This is our implementation of the free function
*/
void free(void *memory) {

  if(memory != NULL) {
    //we're jumping back one chunk position
    struct chunk *cnk = (struct chunk *) ((struct chunk *) memory -1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}




