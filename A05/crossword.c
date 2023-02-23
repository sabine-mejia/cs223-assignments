/*----------------------------------------------
 * Author: Sabine Mejia
 * Date: 02-18-2023
 * Description: This program implements a crossword given two user-generated arguments.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }
  
  int w1_length = strlen(argv[1]); //dictates number of rows
  int w2_length = strlen(argv[2]); //dictates number of columns


  // the 2D array variable is declared to be `int **` (a pointer to an int *)
  // a dynamically allocated array of dynamically allocated int arrays
  // (a pointer to pointers to ints)
  char **two_d_array;
  int s;

  // allocate an array of N pointers to ints
  // malloc returns the address of this array (a pointer to (int *)'s)
  two_d_array = malloc(sizeof(char*) * w1_length);

  // for each row, malloc space for its column elements and add it to
  // the array of arrays
  for (s = 0; s < w1_length; s++) {
  // malloc space for row s's M column elements
      two_d_array[s] = malloc(sizeof(char) * w2_length);
  }
  

  //fill array with dots
  for(int b = 0; b < w1_length; b++) {
    for(int a = 0; a < w2_length; a++) {
      two_d_array[b][a] = '.';
    }
  }  

  //loop through elephant
  for(int i = 0; i < w1_length; i++) {
    
    //loop through onion
    for(int j = 0; j < w2_length; j++) {

      //if the character in elephant matches the character in onion
      if(argv[1][i] == argv[2][j]) { //argv[][] returns an int which is the address to where the char is stored
    
        //print elephant down the jth column
        for(int j2 = 0; j2 < w1_length; j2++) {
          two_d_array[j2][j] = argv[1][j2];
        }  

        //print onion across the ith row
        for(int i2 = 0; i2 < w2_length; i2++) {
          two_d_array[i][i2] = argv[2][i2];
        }
        break;
      }
    }
    printf("No letter in common\n");
  }


  //print two_d_array

  for(int r = 0; r < w1_length; r++) {
    for(int c = 0; c < w2_length; c++) {
        printf("%c", two_d_array[r][c]);
    }
    printf("\n");
  }
  free(two_d_array);

  return 0;
}
