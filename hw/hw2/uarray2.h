/*
 *      uarray2.h
 *      iii
 *      by: Tim Valk (tvalk01) and Valerie Zhang (vzhang02)
 *      date: Sept 24, 2022
 * 
 *      Summary: 
 *      
 */
#include <uarray.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef UARRAY2_T
#define UARRAY2_T

typedef struct UArray2_T *UArray2_T;

        UArray2_T UArray2_new (int width, int length,  int size);
        extern void UArray2_free(UArray2_T *uarray);
        extern int UArray2_get_height(UArray2_T *uarray);
        extern int UArray2_get_width(UArray2_T *uarray);
        UArray_T UArray2_get_array(UArray2_T *uarray);
        extern int UArray2_width(UArray2_T uarray);
        extern int UArray2_height(UArray2_T uarray);
        extern int UArray2_size(UArray2_T uarray);
        extern void *UArray2_at(UArray2_T uarray, int col, int row);
        extern void UArray2_map_row_major(UArray2_T uarray, 
                                          void apply(int col, int row, 
                                                     UArray2_T array, 
                                                     void *elt, void *cl), 
                                          void *cl);
        extern void UArray2_map_col_major(UArray2_T uarray, 
                                          void apply(int i, int j, 
                                                     UArray2_T array, 
                                                     void *elt, void *cl), 
                                          void *cl);

#endif 
