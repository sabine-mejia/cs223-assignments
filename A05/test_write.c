/*----------------------------------------------
 * Author: Sabine Mejia 
 * Date: 02/28/2023
 * Description: This program 
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  
  int w, h;
  const char* filename = "test.ppm";
  struct ppm_pixel** pixels;

  printf("Reading feep-raw.ppm\n");
  pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

  // todo: print out the grid of pixels
  for(int i = 0; i < h; i ++) {
    for(int j = 0; j < w; j ++) {
      printf("(%d,%d,%d) ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }

  // test writing the file to test.ppm, reload it, and print the contents

  printf("Writing to %s \n", filename);
  write_ppm_2d(filename, pixels, w, h);

  pixels = read_ppm_2d("test.ppm", &w, &h);

  // todo: print out the grid of pixels
  printf("Reading %s\n", filename);
  for(int i = 0; i < h; i ++) {
    for(int j = 0; j < w; j ++) {
      printf("(%d,%d,%d) ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }


  free(pixels);
  return 0;
}
