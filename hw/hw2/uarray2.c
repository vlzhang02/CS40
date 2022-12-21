/*
 *      uarray2.c
 *      iii
 *      by: Tim Valk (tvalk01) and Valerie Zhang (vzhang02)
 *      date: Sept 24, 2022
 * 
 *      Summary: 
 *
 */
#include "uarray2.h"
#include <uarray.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct UArray2_T {
        int width;
        int height;
        UArray_T array;
};

/*
 * UArray2_new
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:     stores 
 * Returns:     a UArray2
 */
UArray2_T UArray2_new (int width, int height, int size) 
{
        UArray2_T array_2D = malloc(sizeof(struct UArray2_T));
        assert(array_2D);
        array_2D->width = width;
        array_2D->height = height;
        array_2D->array = UArray_new(width*height, size);
        return array_2D;
}  
/*
 * UArray2_free
 *
 * Purpose:     frees all memory used by a instance of UArray2
 * Parameters:  pointer to UArray2
 * Effects:     frees all memory used by UArray2
 * Returns:     none
 */
void UArray2_free(UArray2_T *uarray)
{
        assert(*uarray);
        UArray_free(&(*uarray)->array);
        free(*uarray);
}

int UArray2_get_height(UArray2_T *uarray)
{
        assert(*uarray);
        return (*uarray)->height;
}

int UArray2_get_width(UArray2_T *uarray)
{
        assert(*uarray);
        return (*uarray)->width;
}

UArray_T UArray2_get_array(UArray2_T *uarray)
{
        assert(*uarray);
        return (*uarray)->array;
}

/*
 * UArray2_width
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
int UArray2_width(UArray2_T uarray)
{
        assert(uarray);
        return uarray->width;
}
/*
 * UArray2_height
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
int UArray2_height(UArray2_T uarray)
{
        assert(uarray);
        return uarray->height;
}

/*
 * UArray2_size
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
 int UArray2_size(UArray2_T uarray)
{
        assert(uarray);
        return UArray_size(uarray->array);
}

 /*
 * UArray2_at
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */

void *UArray2_at(UArray2_T uarray, int col, int row)
{
        assert(uarray);
        int index = uarray->height * col + row;
        return UArray_at(uarray->array, index);
}

/*
 * UArray2_map_col_major
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
extern void UArray2_map_col_major (UArray2_T uarray, void apply(int col, int row, 
                                   UArray2_T array, void *elt, void *cl), 
                                          void *cl)
{
        assert(uarray);
        assert(apply);
        int col_count = 0;
        int row_count = 0;
        for (int i = 0; i < UArray_length(uarray->array); i++) {
                //int col = i - (uarray->height * row_count);
                fprintf(stderr, "\n-------------col: %d------------\n", col_count);
                apply(col_count, row_count, uarray, UArray_at(uarray->array, i), cl);
                if (row_count == uarray->height - 1) {
                        col_count++;
                        row_count = 0;
                } else {
                        row_count++;
                }

        }
}

/*
 * UArray2_map_col_major
 *
 * Purpose:     make new instance of UArray2
 * Parameters:  width and height 
 * Effects:
 * Returns
 */
extern void UArray2_map_row_major (UArray2_T uarray, void apply(int col, int row, 
                                   UArray2_T array, void *elt, void *cl), 
                                          void *cl)
{
        assert(uarray);
        assert(apply);
        for (int r = 0; r < uarray->height; r++) {
                for (int c = 0; c < uarray->width; c++) {
                        int i = uarray->height * c + r;
                        fprintf(stderr, "\n-------------col: %d------------\n", c);
                        apply(c, r, uarray, UArray_at(uarray->array, i), cl);
                }
        }
}