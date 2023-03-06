/*----------------------------------------------
 * Author: Sabine Mejia 
 * Date: 03/04/23
 * Description: This program takes a .ppm file as a command line argument and
 * a message from the user as a runtime argument and embeds the user's message
 * into the .ppm file, outputting the new encoded .ppm file.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
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

//takes in a 1d array and returns a 2d array
struct ppm_pixel** to_two_d_array(struct ppm_pixel* one_d_pixel_arr, int w, int h) {
  struct ppm_pixel two_d_pixel_arr = malloc
} **/

//takes in an integer and returns it as a binary number as a string of integers 
int* int_to_binary(int integer) {
  int* bin_num = malloc(sizeof(int) * 8);
  for(int i = 7; i >= 0; i --) {
    bin_num[i] = (integer % 2);
    integer = integer / 2;
  }   
  for(int i = 0; i < 8; i++) {
  }
  return bin_num; 
}

int main(int argc, char** argv) {
  int w, h;
  const char* filename;
  char* encoded_filename;
  struct ppm_pixel** two_d_pixel_arr;
  //struct ppm_pixel* one_d_pixel_arr;
  char msg[65500];
  int* big_bin_arr;
  int* eight_bit_bin_arr;

  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  filename = argv[1];

  two_d_pixel_arr = read_ppm_2d(filename, &w, &h);
  //one_d_pixel_arr = to_one_d_array(two_d_pixel_arr, w, h);

  printf("Reading %s with width: %d and height: %d\n", filename, w, h);
  printf("Max number of characters in the image: %d\n", w * h * 3 / 8);
  printf("Enter a phrase: ");
  scanf("%s", msg);

  eight_bit_bin_arr = malloc(sizeof(int) * 8);
  big_bin_arr = malloc(sizeof(int) * 8 * strlen(msg));

  int c = 0;
  for(int i = 0; i < strlen(msg); i ++) {
    eight_bit_bin_arr = int_to_binary((int)msg[i]);

    for(int j = 0; j < 8; j ++) {
      big_bin_arr[c] = eight_bit_bin_arr[j];
      c++;
    }
  }

  c = 0;
  int i = 0;
  int j = 0;
  while(c < strlen(msg) * 8) {
    
    printf("old red: %02X\n", two_d_pixel_arr[i][j].red);
    if(two_d_pixel_arr[i][j].red % 2 != big_bin_arr[c]) {
      two_d_pixel_arr[i][j].red += 1;
      printf("new red: %02X\n", two_d_pixel_arr[i][j].red);
    }
    c++;
    if(two_d_pixel_arr[i][j].green % 2 != big_bin_arr[c]) {
      two_d_pixel_arr[i][j].green += 1;
    }
    c++;
    if(two_d_pixel_arr[i][j].blue % 2 != big_bin_arr[c]) {
      two_d_pixel_arr[i][j].blue += 1;
    }
    c++;
    j++;
    if(j > w) {
      i++;
      j = 0;
    }
  }



  //write
  encoded_filename = malloc(sizeof(filename) + sizeof(char) * 9);

  for(int i = 0; i < strlen(filename) - 4; i++) {
    encoded_filename[i] = filename[i];
    printf("%c", encoded_filename[i]);
  }
  strcat(encoded_filename,"_encoded.ppm");
  
  printf("Writing to %s \n", encoded_filename);
  write_ppm_2d(encoded_filename, two_d_pixel_arr, w, h);

  

  return 0;
}

