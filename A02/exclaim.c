#include <stdio.h>
#include <stdlib.h>

int main() {

    char buff[32]; //this array of chars stores user inputted word
    char symbols[5] = {'!','*','#','@','\0'};

    printf("Enter a word:");
    scanf("%s",buff);


    printf("OMG! ");

    for(int i = 0; i< 32; i++) {
        if(buff[i] >= 'a' && buff[i] <= 'z') {
            int randNum = rand() % 4;
            buff[i] = symbols[randNum];
        }
    }

    printf("%s \n",buff);

    return 0;
}
