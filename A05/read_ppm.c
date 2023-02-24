/*----------------------------------------------
 * Author: Sabine Mejia 
 * Date: 02/23/2023
 * Description: This program contains a method that can read PPM files stored 
 * in binary format. This function takes a filename as input and return 
 * a 2D array of struct pixel. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"


// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}


struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  
  FILE* fp;
  char str[1028];

  /* opening file for reading */
  fp = fopen(filename, "r");
   if(fp == NULL) {
    perror("Error opening file");
   }
   if(fgets(str, 60, fp)!= NULL) {
      /* writing content to stdout */
      puts(str);
   }
   fclose(fp);
  
  return NULL;
}
