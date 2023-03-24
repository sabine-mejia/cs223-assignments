#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  struct ppm_pixel** pixel_array;
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  int iter;
  struct ppm_pixel palette[1000];
  struct ppm_pixel color;
  struct ppm_pixel black;
  char filename[100];
  struct timeval tstart, tend;
  double timer;

  black.red = 0;
  black.green = 0;
  black.blue = 0;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here


  //malloc memory for the image
  pixel_array = malloc(sizeof(struct ppm_pixel*) * size);
    if (pixel_array == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        pixel_array[i] = malloc(sizeof(struct ppm_pixel) * size);
        if (pixel_array[i] == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
        }
    }

  //set a random seed for the pallet
  srand(time(0));  

  // generate pallet
  for(int i = 0; i < maxIterations; i++) {

    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  

  // compute image
  gettimeofday(&tstart, NULL);
  for(int row = 0; row < size; row ++) {
    for(int col = 0; col < size; col ++) {
      xfrac = (float)row / size; //or is image_size (size* size)?
      yfrac = (float)col / size;
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);

      x = 0;
      y = 0;
      iter = 0;

      while(iter < maxIterations && x*x + y*y < 2*2) {
        xtmp = x*x - y*y +x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }


      //escaped
      if(iter < maxIterations) {
        color = palette[iter];
      }
      else {
        color = black;
      }

      //write color to image at location (row, col)
      pixel_array[row][col] = color;
    }
  }
  gettimeofday(&tend, NULL);

  timer = tend.tv_sec - tstart.tv_sec;

  // Save the image to a PPM file
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm_2d(filename, pixel_array, size, size); 

  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  for(int i = 0; i < size; i++) {
    free(pixel_array[i]);
  }
  free(pixel_array);
}


