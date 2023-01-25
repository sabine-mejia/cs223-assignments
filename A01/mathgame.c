/***************************************************
 * mathgame.c
 * Author: Sabine Mejia 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
  	
    int rounds, num1, num2, res, ans;
    int counter = 0;

    printf("Welcome to Math Game! \n");
    printf("How many rounds do you want to play? \n");
    scanf("%d", &rounds);
    
    for(int i = 0; i < rounds; i++) {
        num1 = rand() % 9 + 1;
        num2 = rand() % 9 + 1; 
        res = num1 + num2;

    printf("%d + %d = ? ", num1, num2);
    scanf("%d", &ans);

    if(ans == res) {
        printf("Correct! \n");
	printf("\n");
        counter ++;
    }
    else {
        printf("Incorrect :( \n");
	printf("\n");
    }
    }
    printf("You answered %d/%d correctly. \n", counter, rounds);
    
	return 0;
}

