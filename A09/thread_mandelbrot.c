#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct t_arg {
  struct ppm_pixel **pixle_array; // pointer to shared array
  struct ppm_pixel *color_palet; //pointer to color palet
  long id; // logical thread id
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
};

void *GenerateManddelbrot(void *args) {
  //variables
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  int iter;
  struct ppm_pixel color;
  struct ppm_pixel black;

  black.red = 0;
  black.green = 0;
  black.blue = 0;

  //cast to a struct t_arg from void*
  struct t_arg * myargs = (struct t_arg *) args;

  //extract all variables from struct
  struct ppm_pixel **pixel_array = myargs->pixle_array;
  struct ppm_pixel *palet = myargs->color_palet;
  long id = myargs->id;
  int row_start = myargs->row_start;
  int row_end = myargs->row_end;
  int col_start = myargs->col_start;
  int col_end = myargs->col_end;
  int size = myargs->size;
  float xmin = myargs->xmin;
  float xmax = myargs->xmax;
  float ymin = myargs->ymin;
  float ymax = myargs->ymax;
  int maxIterations = myargs-> maxIterations;
  

  for (int row = row_start; row < row_end; row++)
    {
      for (int col = col_start; col < col_end; col++)
      {
        xfrac = (float)row / size; // should I divide this by 4? Or 2?
        yfrac = (float)col / size;
        x0 = xmin + xfrac * (xmax - xmin);
        y0 = ymin + yfrac * (ymax - ymin);

        x = 0;
        y = 0;
        iter = 0;

        while (iter < maxIterations && x * x + y * y < 2 * 2)
        {
          xtmp = x * x - y * y + x0;
          y = 2 * x * y + y0;
          x = xtmp;
          iter++;
        }

        // escaped
        if (iter < maxIterations)
        {
          color = palet[iter];
        }
        else
        {
          color = black;
        }
        // write color to image at location (row, col)
        pixel_array[row][col] = color;
      }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  struct ppm_pixel** pixel_array;
  
  struct ppm_pixel* color_palet;
  struct ppm_pixel color;
  struct ppm_pixel black;
  char filename[100];
  struct timeval tstart, tend;
  double timer;
  int col_start, col_end, row_start, row_end;

  

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here

  //malloc memory for the image pixel array
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

  //malloc memory for the image color palet  
  color_palet = malloc(sizeof(struct ppm_pixel) * maxIterations);

  //set a random seed for the color palet
  srand(time(0)); 

  // generate pallet
  for(int i = 0; i < maxIterations; i++) {
    color_palet[i].red = rand() % 255;
    color_palet[i].green = rand() % 255;
    color_palet[i].blue = rand() % 255;
  }

  //
  pthread_t *thread_array = malloc(4 * sizeof(pthread_t)); //initialize an array of pointers to threads
  struct t_arg *thread_args = malloc(4 * sizeof(struct t_arg)); //initialize an array of pointers to structs

  //set the value of the structs
  for(int i = 0; i < 4; i++) {
    if(i == 0) {
      thread_args[i].row_start = 0;
      thread_args[i].row_end = size/2;
      thread_args[i].col_start = 0;
      thread_args[i].col_end = size/2;
    }
    if(i == 1) {
      thread_args[i].row_start = 0;
      thread_args[i].row_end = size/2;
      thread_args[i].col_start = size/2;
      thread_args[i].col_end = size;
    }
    if(i == 2) {
      thread_args[i].row_start = size/2;
      thread_args[i].row_end = size;
      thread_args[i].col_start = 0;
      thread_args[i].col_end = size/2;
    }
    if(i == 3) {
      thread_args[i].row_start = size/2;
      thread_args[i].row_end = size;
      thread_args[i].col_start = size/2;
      thread_args[i].col_end = size;
    }
    

    thread_args[i].pixle_array = pixel_array;
    thread_args[i].color_palet = color_palet;
    thread_args[i].size = size;
    thread_args[i].id = i;
    thread_args[i].xmin = xmin;
    thread_args[i].xmax = xmax;
    thread_args[i].ymin = ymin;
    thread_args[i].ymax = ymax;
    thread_args[i].maxIterations = maxIterations;
  }

  gettimeofday(&tstart, NULL);
  //initialize threads in thread_array
  for (int i = 0; i < 4; i++) {
    pthread_create(&thread_array[i], NULL, GenerateManddelbrot, &thread_args[i]);
  } 
  for (int i = 0; i < 4; i++) {
    pthread_join(thread_array[i], NULL);
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;  


  //Save the image to a PPM file
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm_2d(filename, pixel_array, size, size); 

  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

}
