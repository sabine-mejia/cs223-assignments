#include <stdio.h>
#include <string.h>

/**
 * The Song component of assignment 03
 *
 * This program fills an array of three or more songs then allows the user to
 * edit the data in the array.
 *
 * @author: Sabine Mejia
 * @version: February 2, 2023
 *
*/

    struct song {
        int durationSecs;
	int durationMins;
	float danceability;
	char artist[32];
	char name[32];
    };

/**
 *
*/

int printSongs(struct song songArray[]) {
    for(int i = 0; i < 3; i++) {

    printf("%d. %s artist: %s duration: %i:%i danceability: %f", i, songArray[i].name, songArray[i].artist, songArray[i].durationMins, songArray[i].durationSecs, songArray[i].danceability);
    }
    return 0;
}	    




int main() {
    
    struct song song1, song2, song3;
    struct song songArray[3];	    
    int songID;	
    char attribute[10];
    char attributesArray[5][10];	

    //declarations for song1
    strcpy(song1.name, "Billions");
    song1.durationMins = 3;
    song1.durationSecs = 37;
    song1.danceability = 0.7;
    strcpy(song1.artist, "Caroline Polacheck");

    //declarations for song2
    strcpy(song2.name, "Catch & Release");
    song2.durationMins = 3;
    song2.durationSecs = 15;
    song2.danceability = 0.3;
    strcpy(song2.artist, "Matt Simons");

    //declarations for song3
    strcpy(song3.name, "Japanese Denim");
    song3.durationMins = 4;
    song3.durationSecs = 30;
    song3.danceability = 0.5;
    strcpy(song3.artist, "Daniel Caesar");


    songArray[1] = song1;
    songArray[2] = song2;
    songArray[3] = song3;

    printf("Welcome to Sabine's song list. \n");
    
    printSongs(songArray);    

    printf("Enter a song id to edit [0,1,2]: \n");
    scanf("%i", songID);	
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability] \n");
    scanf("%s", attribute);
    
    for(int j = 0; j < 5; j++) {
        
        int comp = strcmp(attributesArray[j], attribute);
            if(comp == 0) {
	    printf    
    }	    
    }	    
    return 0;
}
