/*----------------------------------------------
 * Author: Sabine Mejia 
 * Date: 03-03-23
 * Description: This program reads in a unsigned floating point number then
 * converts it to binary and prints an 8 x 8 1bpp image. A 0 in binary 
 * corresponds to a space and a 1 corresponds to an @.
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  unsigned long mask;
  unsigned long masked_img;
  int count = 1;
  
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here
  

  for(int i = 63; i >= 0; i--) {
    mask = 0x1ul << i;
    masked_img = mask & img;
    
    if(masked_img != 0) {
      printf("@");
    }
    if(masked_img == 0) {
      printf(" ");
    }
    if((count % 8 == 0)) {
      printf("\n");
    }
    count ++;
  }
  printf("-----------------------\n");
  return 0;
}
