/**************************************************************
 *
 *                     uarray2.h
 *
 *     Assignment: iii
 *     Authors:  Hannah Fiarman (hfiarm01), Lawer Nyako (lnyako01) 
 *     Date:     2-10-25
 *
 *     Summary: The interface for a 2D version of Hanson's UArray data 
 *              structure. 
 *
 **************************************************************/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <uarray.h>
#include <assert.h>

#define T UArray2_T
typedef struct T *T;

extern T    UArray2_new(int width, int height, int size);
extern void UArray2_free(T *uarray2);

extern int UArray2_width(T uarray2);
extern int UArray2_height(T uarray2);
extern int UArray2_size(T uarray2);

extern void *UArray2_at(T uarray2, int column, int row);

extern void UArray2_map_col_major(T uarray2, void (*apply)(int column, int row,
                                T uarray2, void *element, void *cl),
                                void *cl);
extern void UArray2_map_row_major(T uarray2, void (*apply)(int column, int row,
                                T uarray2, void *element, void *cl),
                                void *cl);

#undef T
#endif