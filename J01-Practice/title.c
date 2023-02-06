#include <stdio.h>


int main () {

    int length;
    char word[128];
    int i,j;

    printf("Please Enter a word: ");
    scanf("%s", &word);
    printf("\n");
    printf("Please enter a length: ");
    scanf("%d", length);

    for(i; i < length; i++) {
	printf("%");
    }	    

    printf("%s", word);

    for(j; j < length; j++) {
        printf("%");
    }
    printf("\n");

}
