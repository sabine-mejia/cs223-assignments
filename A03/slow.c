#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

  int pause;
  char text[32];
  char *ent_arr;		
  int i;
  int j;
  int f = 0;
  int cont;

  printf("Pause length :");
  scanf("%d", &pause);

  printf("Text: ");
  scanf("%s", text);

  ent_arr = malloc(sizeof(char) * (strlen(text) * pause) + strlen(text) + 1);

  for(i = 0; i < strlen(text); i++) {
    ent_arr[f] = text[i];

    cont = pause + strlen(ent_arr);
    printf("%d %d %d %d \n", cont, i, f,j);

    for(j = f + 1; j < cont; j++) {
      ent_arr[j] = '*';
    }

    f += pause + 1;		
  }

  printf("%s \n", ent_arr);

  free(ent_arr);

  return 0; 
}

