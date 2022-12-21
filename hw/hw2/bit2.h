/*
 *      bit2.h
 *      iii
 *      by: Tim Valk (tvalk01) and Valerie Zhang (vzhang02)
 *      date: Sept 26, 2022
 * 
 *      Summary: 
 *      TODO: Move struct definitions to .c files
 */
#include <bit.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BIT2_T
#define BIT2_T

typedef struct Bit2_T *Bit2_T;

        Bit2_T Bit2_new (int width, int length);
        extern void Bit2_free(Bit2_T *barray);
        extern int Bit2_width(Bit2_T barray);
        extern int Bit2_height(Bit2_T barray);
        extern int Bit2_get(Bit2_T barray, int col, int row);
        extern int Bit2_put(Bit2_T barray, int col, int row, int bit);
        Bit_T Bit2_get_array(Bit2_T *barray);
        extern void Bit2_map_row_major(Bit2_T barray, 
                                       void apply(int col, int row, 
                                                  Bit2_T barray, int elt, 
                                                  void *cl), 
                                       void *cl);
        extern void Bit2_map_col_major(Bit2_T barray, 
                                       void apply(int col, int row, 
                                                  Bit2_T barray, int elt, 
                                                  void *cl), 
                                       void *cl);

#endif
