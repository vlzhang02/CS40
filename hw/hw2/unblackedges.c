/*
 *      unblackedges.c
 *      iii
 *      by: Tim Valk (tvalk01) and Valerie Zhang (vzhang02)
 *      date: Sept 24, 2022
 * 
 *      Summary: TODO
 *      
 */

#include "bit2.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>

FILE *open_file(int argc, char *argv[]);
Bit2_T read_and_fill(FILE *fp);

int main (int argc, char *argv[])
{
        FILE *fp = open_file(argc, argv);
        Bit2_T barray = read_and_fill(fp);
        (void) barray;
        
}

/*
 * open_file()
 *
 * Purpose:     check arguments 
 * Parameters:  
 * Effect:      
 * Returns:     
 */
FILE *open_file(int argc, char *argv[]) {
        assert(argc < 3);
        FILE *inputfd; // open file
        if (argc == 1){
                inputfd = stdin;
        } else {
                inputfd = fopen(argv[1], "rb");
                if (inputfd == NULL) {
                        assert(inputfd != NULL);
                }
        } 
        return inputfd;
}

Bit2_T read_and_fill(FILE *fp)
{
        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata mapdata = Pnmrdr_data(reader);
        Bit2_T barray = Bit2_new(mapdata.width, mapdata.height);
        assert((mapdata.type == 4) || (mapdata.type == 1));
        for (size_t row = 0; row < mapdata.height; row++) {
                for (size_t col = 0; col < mapdata.width; col++) {
                        Bit2_put(barray, col, row, Pnmrdr_get(reader));
                        fprintf(stderr, "Reading in: %d\n", Bit2_get(barray, col, row));
                        assert((Bit2_get(barray, col, row) == 0) || 
                               (Bit2_get(barray, col, row) == 1));
                }
        }
        fclose(fp);
        Pnmrdr_free(&reader);
        return barray;
}