/*----------------------------------------------
 * Author: Sabine Mejia
 * Date: 03/05/23
 * Description: This program implements the method write_ppm_2d which takes
 * a filename, 2D pixel array, and width and height as parameters. write_ppm_2d
 * then opens the file and writes the information contained in the 2D pixel array
 * into the file in hexidecimal.
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

    FILE * fp;
    fp = fopen(filename, "w+");
    fprintf(fp, "P6\n#Comment\n%d %d\n255\n", w, h);
   
    for(int i = 0; i < h; i ++) {
        for(int j = 0; j < w; j++) {
            fwrite(&pixels[i][j], 1, 3, fp); //or replace 3 with 3* *w * *h???
        }
    }
    
    fclose(fp);
}
