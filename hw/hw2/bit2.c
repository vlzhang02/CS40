/*
 *      bit2.c
 *      iii
 *      by: Tim Valk (tvalk01) and Valerie Zhang (vzhang02)
 *      date: Sept 26, 2022
 * 
 *      Summary: 
 *
 */

 #include "bit2.h"
 #include <bit.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <assert.h>

struct Bit2_T {
        int width;
        int height;
        Bit_T array;
};

 /*
 * Bit2_new
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:     stores 
 * Returns
 */
Bit2_T Bit2_new (int width, int height) 
{
        Bit2_T array_2D = malloc(sizeof(struct Bit2_T));
        assert(array_2D);
        array_2D->width = width;
        array_2D->height = height;
        array_2D->array = Bit_new(width*height);
        return array_2D;
}  
/*
 * Bit2_free
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height of 2D array and size of each element
 * Effects:     stores width and height 
 * Returns
 */
void Bit2_free(Bit2_T *barray)
{
        assert(*barray);
        Bit_free(&(*barray)->array);
        free(*barray);
}

/*
 * Bit2_get
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
int Bit2_get(Bit2_T barray, int col, int row)
{
        assert(barray);
        int index = barray->height * col + row;
        return Bit_get(barray->array, index);
}

/*
 * Bit2_put
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
int Bit2_put(Bit2_T barray, int col, int row, int bit)
{
        assert(barray);
        int index = barray->height * col + row;
        int prev = Bit_put(barray->array, index, bit);
        return prev;
}

/*
 * Bit2_map_col_major
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
void Bit2_map_col_major (Bit2_T barray, void apply(int col, int row, 
                                   Bit2_T barray, int elt, void *cl), 
                                          void *cl)
{
        assert(barray);
        assert(apply);
        int col_count = 0;
        int row_count = 0;
        for (int i = 0; i < Bit_length(barray->array); i++) {
                //int col = i - (uarray->height * row_count);
                fprintf(stderr, "\n-------------col: %d------------\n", col_count);
                apply(col_count, row_count, barray, Bit_get(barray->array, i), cl);
                if (row_count == barray->height - 1) {
                        col_count++;
                        row_count = 0;
                } else {
                        row_count++;
                }

        }
}

/*
 * Bit2_map_col_major
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
void Bit2_map_row_major (Bit2_T barray, void apply(int col, int row, 
                                   Bit2_T barray, int elt, void *cl), 
                                          void *cl)
{
        assert(barray);
        assert(apply);
        for (int r = 0; r < barray->height; r++) {
                for (int c = 0; c < barray->width; c++) {
                        int i = barray->height * c + r;
                        fprintf(stderr, "\n-------------col: %d------------\n", c);
                        apply(c, r, barray, Bit_get(barray->array, i), cl);
                }
        }
}

int Bit2_height(Bit2_T barray)
{
        assert(barray);
        return barray->height;
}

int Bit2_width(Bit2_T barray)
{
        assert(barray);
        return barray->width;
}

Bit_T Bit2_get_array(Bit2_T *barray)
{
        assert(barray);
        return (*barray)->array;
}