#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels , int w, int h) {
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
