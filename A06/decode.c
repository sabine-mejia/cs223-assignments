
/*----------------------------------------------
 * Author: Sabine Mejia 
 * Date: 03-03-23
 * Description: This program reads in a .ppm file and extracts a secrete message
 * from it by recording the last digit of every color stream in a pixel and
 * then for every 8 binary numbers finding the corresponding ASCII character.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <string.h>

//takes in a 2d array and returns a 1d array
unsigned char* to_one_d_array(struct ppm_pixel** two_d_pixel_arr, int w, int h) {
  unsigned char* one_d_arr = malloc(sizeof(unsigned char) * w * h * 3);

  int c = 0;
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      one_d_arr[c] = (unsigned char)two_d_pixel_arr[i][j].red;
      
      one_d_arr[c + 1] = (unsigned char)two_d_pixel_arr[i][j].green; 
      
      one_d_arr[c + 2] = (unsigned char)two_d_pixel_arr[i][j].blue;
      c += 3;
    }
  }
  return one_d_arr;
}


// int fromBinary(char *s) {
//   return (int) strtol(s, NULL, 2);
// } 

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel** two_d_pixel_arr;
  unsigned char* one_d_pixel_arr; 
  const char* filename;
  int* bin_str;
  //int* eight_bit_str;
  int integer;

  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  filename = argv[1];
  two_d_pixel_arr = read_ppm_2d(filename, &w, &h);
  one_d_pixel_arr = to_one_d_array(two_d_pixel_arr, w, h);

  printf("Reading %s with width: %d and height: %d\n", filename, w, h);
  printf("Max number of characters in the image: %d\n", w * h * 3 / 8);


  bin_str = malloc(sizeof(int) * 3 * w * h + 1);
  for(int i = 0; i < w * h * 3; i++) {
    if(one_d_pixel_arr[i] % 2 == 0) {
      bin_str[i] = 0;
    }
    else {
      bin_str[i] = 1;
    }
    if(one_d_pixel_arr[i] % 2 == 0) {
      bin_str[i] = 0;
    }
    else {
      bin_str[i] = 1;
    }
    if(one_d_pixel_arr[i] % 2 == 0) {
      bin_str[i] = 0;
    }
    else {
      bin_str[i] = 1;
    }
  }


for(int i = 0; i < 8; i++) {
        printf("%d", bin_str[i]);
        
    }
    printf("\n");

  for(int i = 0; i < 3 * w * h; i+= 8) {
    // if(i % 8 != 0) {
    //   eight_bit_str[c] = bin_str[i];
    //   c++;
    // }
    
    //if(i % 8 == 0) {

    // for(int i = 0; i < 8; i++) {
    //     printf("%d", eight_bit_str[i]);
    // }
    // printf("\n");

      integer = 0;
      integer += 1 * bin_str[i + 7];
      integer += 2 * bin_str[i + 6];
      integer += 4 * bin_str[i + 5];
      integer += 8 * bin_str[i + 4];
      integer += 16 * bin_str[i + 3];
      integer += 32 * bin_str[i + 2];
      integer += 64 * bin_str[i + 1];
      integer += 128 * bin_str[i];

      printf("%c", integer);
    
  }

  free(one_d_pixel_arr);
  free(bin_str);
  //free(eight_bit_str);

  return 0;
}

