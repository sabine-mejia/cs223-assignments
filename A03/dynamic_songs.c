/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

struct Song {
  char title[32];
  char artist[32];
  int duration; //in ms
  float danceability;
  float energy;
  float tempo;
  float valence;  
};


int main() {
  
  FILE *infile;  
  char *token;
  
  infile = fopen("songlist.csv", "r");
  if(infile == NULL) {
  printf("Error: unable to open file %s\n", "songlist.csv");
}
  
  struct Song* song_array = (struct Song*)malloc(sizeof(struct Song) * num_rows);

  char row[1000];
  fgets(row, 1000, infile);  //read number of songs;
  int num_row = atoi(row);  
  printf("%d", num_row); //in for loop, get each row and store it in the songs array then print it

  fgets(row, 1000, infile); // read header
  
  for(int i; i < num_rows; i ++) {
    fgets(row, 1000, infile); //read each song
    
    token = strtok(row, ",");

    while(token != NULL) {
      printf("%s\n", token ); //printing each token
      token = strtok(NULL, " ");
   }


  }
  fclose(infile);
  return 0;

}
