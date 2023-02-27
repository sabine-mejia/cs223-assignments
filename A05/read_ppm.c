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
  char* token;

  

  /* opening file for reading */
  fp = fopen(filename, "rb");
   if(fp == NULL) {
    perror("Error opening file");
   }
   //reads 1st line of header
   if(fgets(str, 1028, fp)!= NULL) {
   }
   //read 2nd line of header
   if(fgets(str, 1028, fp)!= NULL) {
   }
   //read 3rd line of header (this contains the dimenstions)
   if(fgets(str, 1028, fp) != NULL) {
      token = strtok(str, " ");
      *w = atoi(token);

      token = strtok(NULL, " ");
      *h = atoi(token);
      
      printf("dimensions: %d %d \n", *w, *h);
   }
   //read 4th line of header
   if(fgets(str, 1028, fp)!= NULL) {
   }

   //read rest of file
   if(fgets(str, 1028, fp) != NULL) {

      printf("hi %s", str);
      token = strtok(str, " "); //get first token
      puts(str);
      puts(token);

      //declare and allocate space for a 2D array of type struct pointer
      struct ppm_pixel** pixel_array;
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
      printf("%u \n", pixel_array[3][3].red);


      for(int row = 0; row < *h; row++) {

         for(int col = 0; col < *w; col++) {

            struct ppm_pixel new_pixel;

            fread(&new_pixel, sizeof(struct ppm_pixel), 1, fp);

            pixel_array[row][col] = new_pixel;
            /**
            pixel_array[row][col].red = *token;
            token = strtok(NULL, " ");
            pixel_array[row][col].green = *token;
            token = strtok(NULL, " ");
            pixel_array[row][col].blue = *token;
            token = strtok(NULL, " ");
            **/
         }
      }

   } 
  
   

   

   fclose(fp);
  
  return NULL;


  //malloc a 2D array of ppm_pixel structs
  

}
