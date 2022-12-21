/*
 *      sudoku.c
 *      iii
 *      by: Tim Valk (tvalk01) and Valerie Zhang (vzhang02)
 *      date: Sept 24, 2022
 * 
 *      Summary: TODO
 *      
 */

#include "uarray2.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include <set.h>
#include <stdbool.h>

FILE *open_file(int argc, char *argv[]);
void correctness_check(UArray2_T uarray);
UArray2_T read_and_fill(FILE *fp);
int box_num(int row, int col);
bool is_in_array(int val, int array[]);

int main(int argc, char *argv[])
{
        FILE *fp = open_file(argc, argv);
        UArray2_T uarray = read_and_fill(fp);
        correctness_check(uarray);
        // UArray2_free(&uarray);
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

UArray2_T read_and_fill(FILE *fp)
{
        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata mapdata = Pnmrdr_data(reader);
        UArray2_T uarray = UArray2_new(mapdata.width, mapdata.height, sizeof(int));
        assert(mapdata.type == 2);
        assert(mapdata.width == 9);
        assert(mapdata.height == 9);
        for (size_t row = 0; row < mapdata.height; row++) {
                for (size_t col = 0; col < mapdata.width; col++) {
                        *(int *)UArray2_at(uarray, col, row) = Pnmrdr_get(reader);
                        //fprintf(stderr, "Reading in: %d\n", *(int *)UArray2_at(uarray, col, row));
                        assert((*(int *)UArray2_at(uarray, col, row) > 0) && 
                               (*(int *)UArray2_at(uarray, col, row) <= 9));
                }
        }
        fclose(fp);
        Pnmrdr_free(&reader);
        return uarray;
}

void correctness_check(UArray2_T uarray)
{
        int box_array[81];
        for (int i = 0; i < 81; i++) {
                box_array[i] = i + 1;
        }
        for (int col = 0; col < UArray2_get_height(&uarray); col++) {
                int row_array[9]= {1, 2, 3, 4, 5, 6, 7, 8, 9};
                int col_array[9]= {1, 2, 3, 4, 5, 6, 7, 8, 9};
                for (int row = 0; row < UArray2_get_height(&uarray); row++) {
                        int col_val = *(int *)UArray2_at(uarray, col, row);
                        int row_val = *(int *)UArray2_at(uarray, row, col);
                        int square_val = row_val + box_num(row, col) * 9;
                        if ((!is_in_array(col_val, col_array)) || 
                            (!is_in_array(row_val, row_array)) || 
                            (!is_in_array(square_val, box_array))) {
                                UArray2_free(&uarray);
                                exit(1);
                        }

               }
        }
}

int box_num(int row, int col) 
{
        if (row < 3) {
                if (col < 3) {
                        return 0;
                } else if ((col >= 3) && (col < 6)) {
                        return 1;
                } else if ((col >= 6) && (col < 9)) {
                        return 2;
                }
        }
        if ((row < 6) && (row >= 3)) {
                if (col < 3) {
                        return 3;
                } else if ((col >= 3) && (col < 6)) {
                        return 4;
                } else if ((col >= 6) && (col < 9)) {
                        return 5;
                }
        }
        if (row >= 6) {
                if (col < 3) {
                        return 6;
                } else if ((col >= 3) && (col < 6)) {
                        return 7;
                } else if ((col >= 6) && (col < 9)) {
                        return 8;
                }
        }
        return -1;
}

bool is_in_array(int val, int array[])
{
        if (array[val - 1] == val) {
                array[val - 1] = 0;
                return true;
        } else {
                return false;
        }
}

