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
struct ppm_pixel* to_one_d_array(struct ppm_pixel** two_d_pixel_arr, int w, int h) {
  struct ppm_pixel* one_d_arr = malloc(sizeof(struct ppm_pixel) * w * h);

  int c = 0;
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      one_d_arr[c] = two_d_pixel_arr[i][j];
      c++;
    }
  }
  return one_d_arr;
}

int fromBinary(char *s) {
  return (int) strtol(s, NULL, 2);
}

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel** two_d_pixel_arr;
  struct ppm_pixel* one_d_pixel_arr; 
  const char* filename;
  char* bin_str;
  char* eight_bit_str;
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


  bin_str = malloc(sizeof(int) * 3 * w * h);
  int c = 0;
  for(int i = 0; i < w * h; i++) {
    if(one_d_pixel_arr[i].red % 2 == 0) {
      bin_str[c] = '0';
    }
    else {
      bin_str[c] = '1';
    }
    c++;
    if(one_d_pixel_arr[i].green % 2 == 0) {
      bin_str[c] = '0';
    }
    else {
      bin_str[c] = '1';
    }
    c++;
    if(one_d_pixel_arr[i].blue % 2 == 0) {
      bin_str[c] = '0';
    }
    else {
      bin_str[c] = '1';
    }
    c++;
  }

  for(int i = 0; i < 3 * w * h; i++) {
  }

  eight_bit_str = malloc(sizeof(char) * 8);
  c = 0;
  for(int i = 0; i < 3 * w * h; i++) {
    if(c < 8) {
      eight_bit_str[c] = bin_str[i];
      c++;
    }
    
    if(c == 8) {
      integer = fromBinary(eight_bit_str);
      printf("%c", integer);
      c = 0;
    }
  }
  printf("\n");

  free(one_d_arr);
  free(bin_str);
  free(eight_bit_str);

  return 0;
}

