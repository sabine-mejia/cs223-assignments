#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <math.h>

struct t_arg {
  struct ppm_pixel **pixel_array; // pointer to shared array
  //struct ppm_pixel *color_palet; //pointer to color palet
  int **escaped_status_arr; //pointer to boolean array
  int **count_arr;
  long id; // logical thread id
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  int size;
  int *max_count;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
};

void * generateBuddhabrot(void* args) {

  //variables
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  int row, col, yrow, xcol, value, iter;
  struct ppm_pixel color;

  //cast to a struct t_arg from void*
  struct t_arg * myargs = (struct t_arg *) args;

  //extract variables from args struct
  struct ppm_pixel **pixel_array = myargs->pixel_array;
  int **escaped_status_arr = myargs->escaped_status_arr; //pointer to boolean array
  int **count_arr = myargs->count_arr;
  int *max_count = myargs->max_count;
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

  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  // step 1: determine mandelbrot set membership

  for (row = row_start; row < row_end; row++)
    {
      for (col = col_start; col < col_end; col++)
      {
        
        xfrac = (float)row / size; 
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
          escaped_status_arr[row][col] = 0; //set membership at (row,col) to false
        }
        else
        {
          escaped_status_arr[row][col] = 1; //set membership at (row,col) to true
        }
      }
    }

  // perform step 2
    for(row = row_start; row < row_end; row++) {
      for(col = col_start; col < col_end; col++) {

        //if(row,col) belongs to the mandelbrot set, continue
        if((escaped_status_arr[row][col]) == 1) {
          xfrac = (float)row / (float)size; 
          yfrac = (float)col / (float)size;

          x0 = xmin + xfrac * (xmax - xmin);
          y0 = ymin + yfrac * (ymax - ymin);

          x = 0;
          y = 0;

          while(x*x + y*y + 5 < 2*2) { //x + y are really small for some reason, so I added 5 to make the program run ...
            
            xtmp = x*x - y*y + x0;
            y = 2*x*y + y0;
            x = xtmp;

            yrow = round(size * (y - ymin) / (ymax-ymin));
            xcol = round(size * (x - xmin) / (xmax-xmin));

            //in range 
            if(yrow >= 0 && yrow < size) {

              if(xcol >= 0 && xcol < size) {

                pthread_mutex_lock(&mutex);
                count_arr[yrow][xcol]++; //put mutex around count array

                if(count_arr[yrow][xcol] > *max_count) { 

                  *max_count = count_arr[yrow][xcol];
                }
                pthread_mutex_unlock(&mutex);

              } // end of 3rd if

            } // end of 2nd if

          } // end of while.... doesn't make it out of this one

        }
      }
    }
    



  // perform step 3
  long gamma = 0.681;
  long factor = 1.0/gamma;

  for(row = row_start; row < row_end; row++) {
      for(col = col_start; col < col_end; col++) {
         value = 0;

         if(count_arr[row][col] > 0) {
          value = log(count_arr[row][col]) / log((double) *max_count);
          value = pow(value, factor);

          color.red = value * 255;
          color.green = value * 255;
          color.blue = value * 255;

          pixel_array[row][col].red = color.red;
          pixel_array[row][col].green = color.green;
          pixel_array[row][col].blue = color.blue;

         }
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
  int *max_count = malloc(sizeof(int) * 1); //pointer to shared max count

  struct ppm_pixel **pixel_array; // pointer to shared array
  //struct ppm_pixel *color_palet; //pointer to color palet
  int **escaped_status_arr; //pointer to boolean array
  int **count_arr;

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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image

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

    //malloc memory for the boolean array
    escaped_status_arr = malloc(sizeof(int*) * size);
    if (escaped_status_arr == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        escaped_status_arr[i] = malloc(sizeof(int) * size);
        if (escaped_status_arr[i] == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
        }
    }
    //malloc memory for the boolean array
    count_arr = malloc(sizeof(int*) * size);
    if (count_arr == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        count_arr[i] = malloc(sizeof(int) * size);
        if (count_arr[i] == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
        }
    }

    //allocate memory to prepare for threads
    pthread_t *thread_array = malloc(4 * sizeof(pthread_t)); //initialize an array of pointers to threads
    struct t_arg *thread_args = malloc(4 * sizeof(struct t_arg)); //initialize an array of pointers to structs

    //set value of the structs
    for(int i = 0; i < 4; i++) {
    if(i == 0) {
      thread_args[i].row_start = 0;
      thread_args[i].row_end = size/2;
      thread_args[i].col_start = 0;
      thread_args[i].col_end = size/2;
    }
    //Q2
    if(i == 1) {
      thread_args[i].row_start = 0;
      thread_args[i].row_end = size/2;
      thread_args[i].col_start = size/2;
      thread_args[i].col_end = size;
    }
    //Q3
    if(i == 2) {
      thread_args[i].row_start = size/2;
      thread_args[i].row_end = size;
      thread_args[i].col_start = 0;
      thread_args[i].col_end = size/2;
    }
    //Q4
    if(i == 3) {
      thread_args[i].row_start = size/2;
      thread_args[i].row_end = size;
      thread_args[i].col_start = size/2;
      thread_args[i].col_end = size;
    }
    
    thread_args[i].pixel_array = pixel_array;
    thread_args[i].size = size;
    thread_args[i].escaped_status_arr = escaped_status_arr;
    thread_args[i].count_arr = count_arr;
    thread_args[i].id = i;
    thread_args[i].xmin = xmin;
    thread_args[i].xmax = xmax;
    thread_args[i].ymin = ymin;
    thread_args[i].ymax = ymax;
    thread_args[i].max_count = max_count;
    thread_args[i].maxIterations = maxIterations;
  }

  gettimeofday(&tstart, NULL);

  //initialize threads in thread_array
  for(int i = 0; i < 4; i++) {
    pthread_create(&thread_array[i], NULL, generateBuddhabrot, &thread_args[i]);

    printf("Thread %ld) sub-image block: cols (%d, %d) to rows (%d,%d)\n", 
    thread_args[i].id, thread_args[i].col_start, thread_args[i].col_end, 
    thread_args[i].row_start, thread_args[i].row_end);
  } 
  for (int i = 0; i < 4; i++) {
    printf("Thread %ld) Finished\n", thread_args[i].id);
    pthread_join(thread_array[i], NULL);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;  

  //Save the image to a PPM file
  sprintf(filename, "buddhabrot-%d-%ld.ppm", size, time(0));
  write_ppm_2d(filename, pixel_array, size, size); 

  printf("Computed buddhabrot set (%dx%d) in %.6f seconds\n", size, size, timer);
  printf("Writing file: buddhabrot-%d-%ld.ppm\n", size, time(0));

  for(int i = 0; i < size; i++) {
    free(pixel_array[i]);
  }
  free(pixel_array);
  free(thread_array);
  free(thread_args);
  free(max_count);
  free(escaped_status_arr);
  free(count_arr);

}
