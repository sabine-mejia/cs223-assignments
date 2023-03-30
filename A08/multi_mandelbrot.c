#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char *argv[])
{
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  pid_t pid, pid_num;
  int child_status;
  int iter, i, j;
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  struct ppm_pixel color;
  struct ppm_pixel black;
  char filename[100];
  struct timeval tstart, tend;
  double timer;

  black.red = 0;
  black.green = 0;
  black.blue = 0;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1)
  {
    switch (opt)
    {
    case 's':
      size = atoi(optarg);
      break;
    case 'l':
      xmin = atof(optarg);
      break;
    case 'r':
      xmax = atof(optarg);
      break;
    case 't':
      ymax = atof(optarg);
      break;
    case 'b':
      ymin = atof(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> "
             "-b <ymin> -t <ymax> -p <numProcesses>\n",
             argv[0]);
      break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here

  int shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*)*size*size, IPC_CREAT | 0665);

  // attach shared memory
  struct ppm_pixel *shm_ptr = (struct ppm_pixel *) shmat(shmid, NULL, 0);

  // create 2D array in shared memory
  struct ppm_pixel **data = (struct ppm_pixel **) malloc(size * sizeof(struct ppm_pixel *));
  for (i = 0; i < size; i++) {
      data[i] = shm_ptr + i * size;
  } 

  //allocate shared memory for pallet
  int shmid2 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * maxIterations, 0645 | IPC_CREAT);
  if (shmid2 == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }
  struct ppm_pixel *palette = shmat(shmid2, NULL, 0);
  if (palette == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 


  // generate pallet
  for (int i = 0; i < maxIterations; i++)
  {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // set a random seed for the pallet
  srand(time(0));


  gettimeofday(&tstart, NULL);
  // compute image
  pid = fork();

  // Child 1: Q2
  if (pid == 0)
  {
    pid_num = getpid();
    printf("Launched child process: %d\n", pid_num);

    pid = fork();

    // Grandchild: Q3
    if (pid == 0)
    {
      pid_num = getpid();
      printf("Launched child process: %d\n", pid_num);

      pid = fork();

      // Great-Grand child: Q4
      if (pid == 0)
      {
        //Q4
        pid_num = getpid();
        printf("Launched child process: %d\n", pid_num);
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", pid_num, size / 2, size, size / 2, size);
        for (int row = size/2; row < size; row++)
        {
          for (int col = size/2; col < size; col++)
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
              color = palette[iter];
            }
            else
            {
              color = black;
            }

            // write color to image at location (row, col)
            data[row][col] = color;
          }
        }
      }
      else
      //Q3
      {
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", pid_num, 0, size / 2, size / 2, size);
        for (int row = size/2; row < size; row++)
        {
          for (int col = 0; col < size / 2; col++)
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
              color = palette[iter];
            }
            else
            {
              color = black;
            }

            // write color to image at location (row, col)
            data[row][col] = color;
          }
        }

        pid = wait(&child_status);
      }
    }
    else
    //Q2
    {
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", pid_num, size / 2, size, 0, size / 2);
      for (int row = 0; row < size / 2; row++)
      {
        for (int col = size/2; col < size; col++)
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
            color = palette[iter];
          }
          else
          {
            color = black;
          }

          // write color to image at location (row, col)
          data[row][col] = color;
        }
      }
      pid = wait(&child_status);
      // write ppm?
    }
  }

  else
  {
    // Q1
    pid_num = getpid();
    printf("Launched parent process: %d\n", pid_num);
    printf("%d) Sub-image block: cols (0, %d) to rows (0, %d)\n", pid_num, size / 2, size / 2);

    // calculate mandelbrot
    for (int row = 0; row < size / 2; row++)
    {
      for (int col = 0; col < size / 2; col++)
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
          color = palette[iter];
        }
        else
        {
          color = black;
        }

        // write color to image at location (row, col)
        data[row][col] = color;
      }
    }
    pid = wait(&child_status);
    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

    sprintf(filename, "multi-mandelbrot-%d-%ld.ppm", size, time(0));
    write_ppm_2d(filename, data, size, size);
    printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);
    
  }
    

  // detach shared memory
  shmdt(shm_ptr);
  shmctl(shmid, IPC_RMID, NULL);

  shmdt(palette);
  shmctl(shmid2, IPC_RMID, NULL);
}

