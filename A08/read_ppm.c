#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {

  return NULL;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
FILE* fp;
  char str[1028];
  struct ppm_pixel** pixel_array; 

  /* opening file for reading */
  fp = fopen(filename, "rb");
   if(fp == NULL) {
    perror("Error opening file");
   }
   //reads 1st line of header
   fgets(str, 1028, fp);
   //read 2nd line of header
   fgets(str, 1028, fp);
   //read 3rd line of header (this contains the dimenstions)
   fgets(str, 1028, fp);
   sscanf(str, "%d %d", w, h);
   //read 4th line of header
   fgets(str, 1028, fp);

      //declare and allocate space for a 2D array of type struct pointer
      
      pixel_array = malloc(sizeof(struct ppm_pixel*) * *h);
      if (pixel_array == NULL) {
         printf("Error: malloc failed\n");
         exit(1);
      }
      for(int i = 0; i < *h; i++) {
         pixel_array[i] = malloc(sizeof(struct ppm_pixel) * *w);
         if (pixel_array[i] == NULL) {
         printf("Error: malloc failed\n");
         exit(1);
         }
      }

   //read rest of file and store it in struct
   for(int i = 0; i < *h; i ++) {
      for(int j = 0; j < *w; j++) {
         fread(&(pixel_array[i][j]), sizeof(struct ppm_pixel), 1, fp);
      }
   }
      

   fclose(fp);
  
  return NULL;
}
