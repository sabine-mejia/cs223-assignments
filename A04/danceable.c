/*----------------------------------------------
 * Author: Sabine Mejia
 * Date: 02-15-2023 
 * Description: This program reads in a csv file containing songs and their
 * attributes and then adds each song to a linked list data structure. It then
 * prints the song with the highest danceability score each time the user 
 * requests it. Each time a song is printed, it is removed from the linked list.

 STEPS:
 0. Declare a linked list -> done
 1. Read in csv file
 2. Read each row with fgets until fgets returns null (while fgets != NULL)
 3. Store each Song struct in a node
 4. Use a temp to store the highest danceability song and then after you print
 it, delete it
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Song {
  char title[32];
  char artist[32];
  int duration; //in ms
  float danceability;
  float energy;
  float tempo;
  float valence;  
};

struct node {
  struct Song song; //this node holds a Song struct called song
  struct node* next; //pointer to next node
};

//This function returns the number of nodes in a list given the list's head
int count(struct node* list) {
  int i = 0;
  for(struct node* n = list; n != NULL; n=n->next) {
    i++;
  }
  return i;
}

//This function prints the values held in a linked list
void print(struct node* list) {
  int i = 0;
  for(struct node* n = list; n != NULL; n=n->next) {
    printf("%d%s %-30s artist: %-20s duration: %d D: %.3f E: %.3f T: %.3f V: %.3f \n", i, ")", n->song.title, n->song.artist, 
    n->song.duration, n->song.danceability, n->song.energy,
    n->song.tempo, n->song.valence);
    i++;
  }
}

//This function takes in the head of a linked list, and the name of a node as an argument and deletes the node from the list
struct node* delete_node(struct node* list, struct node* rm) {
  for(struct node* n = list; n != NULL; n=n->next) {
    if(n->song.danceability == rm->song.danceability) {
      list->next = rm->next;
      rm = n;
    }
    list = n;
  }
  return rm;
}

//This function traverses the linked list and finds the song with the max danceability
struct node* find_max_danceability(struct node* list) {
  struct node* most_danceable = list;
  for(struct node* n = list; n != NULL; n=n->next) {
    if(n->song.danceability > most_danceable->song.danceability) {
      most_danceable = n; 
    }
    list = n;
  }
  //printf("%f \n", most_danceable->song.danceability);
  return most_danceable;
}

//This function inserts a node to the front of the linked list
struct node* insert_front(struct Song inserted_song, struct node* head) {

  struct node* n = malloc(sizeof(struct node)); //allocates space for a node called node n

  if(n == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  n->song = inserted_song; //the variable song in the node n is set to the value of the Song struct inserted_song Song in the parameters
  n->next = head; //the pointer next in the node n points at the head of the linked list
  return n; //returns the new node which is the new head
}

int main() {
  FILE* infile;  
  char* token;  
  char row[1000];
  char input;
	int num_songs;

  //struct node* new_node;
  struct node* head_ptr = NULL;
  int i = 0;

  //Open and read songlist csv
  infile = fopen("songlist.csv", "r");
  if(infile == NULL) {
  printf("Error: unable to open file %s\n", "songlist.csv");
  }
  
  fgets(row, 1000, infile); // read header

  //read each row of the csv file
  while(fgets(row, 1000, infile) != NULL) {
    i = 0;
    //declare a new Song struct
    struct Song new_song_struct;

    token = strtok(row, ","); //get first token in the row


    //loop through rest of tokens in the row
    while(token != NULL) {

      if(i == 0) { 
      strcpy(new_song_struct.title, token); //store the title in the song struct

      }
      if(i == 1) {
       strcpy(new_song_struct.artist, token);
      }
      if(i == 2) {
        new_song_struct.duration = atoi(token);
      }
      if(i == 3) {
        new_song_struct.danceability = atof(token);
      }
      if(i == 4) {
        new_song_struct.energy = atof(token);
      }
      if(i == 5) {
        new_song_struct.tempo = atof(token);
      }
      if(i == 6) {
        new_song_struct.valence = atof(token);
      }

      i++;

      token = strtok(NULL, ","); //move to next token?
    }

  /*
  printf("%s %s %d %.3f %.3f %.3f %.3f \n", new_song_struct.title, new_song_struct.artist, 
  new_song_struct.duration, new_song_struct.danceability, new_song_struct.energy,
  new_song_struct.tempo, new_song_struct.valence);
  */

  //insert a new node
  head_ptr = insert_front(new_song_struct, head_ptr);

  } //end of while loop
  fclose(infile);

  //print linked list
  printf("\n");
  print(head_ptr);
  printf("\n");
  printf("This dataset contains %d songs \n", count(head_ptr));
  printf("\n");
  printf ("--------------------------------------------\n");
  printf ("--------------------------------------------\n");
  printf("\n");
  
  printf("Press 'd' to show the most danceable song (any other key to quit): ");  
  
  scanf("%c", &input);
  printf("\n");

	
  if(input == 'd') {


		for(num_songs = count(head_ptr); num_songs > 0; num_songs += 0) {
			if(input == 'd') { 
			//find most danceable song
			struct node* max_danceability_node = find_max_danceability(head_ptr);

			

			//print the most danceable node
			printf ("-------------------------------------------------- Most Danceable --------------------------------------------------\n");
			print(max_danceability_node);
			printf ("--------------------------------------------------------------------------------------------------------------------\n");

			//delete most danceable node
			delete_node(head_ptr, max_danceability_node);

			//print linked list
			print(head_ptr);
			printf("\n");
			printf("This dataset contains %d songs \n", count(head_ptr));
			printf("\n");
			printf ("--------------------------------------------\n");
			printf ("--------------------------------------------\n");
			printf("\n");

			printf("Press 'd' to show the most danceable song (any other key to quit): ");  
			scanf("%c %*c", &input);
			printf("%c hi",input); //problem here
			printf("\n");

			num_songs = count(head_ptr);
			}
		}
  }
  return 0;
}
