/*----------------------------------------------
 * Author: Sabine Mejia 
 * Date: 02/28/2023
 * Description: This program calls the method read_ppm_2d to test it. It passes
 * read_ppm_2d the name of a binary file to read from along with the addresses 
 * of where to store the height and width of the pixel array the file contains.
 * It then prints each element of the 2D array returned by read_ppm_2d.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;

  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);


  // todo: print out the grid of pixels
  for(int i = 0; i < h; i ++) {
    for(int j = 0; j < w; j ++) {
      printf("(%d,%d,%d) ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }

  for(int i = 0; i < h; i++) {
    free(pixels[i]);
  }
  free(pixels);
  return 0;
}

