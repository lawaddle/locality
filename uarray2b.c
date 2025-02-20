
#include "uarray2.h"
#include "uarray2b.h"
#include <math.h>
#include <uarray.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#define T UArray2b_T

struct T {
        UArray2_T array;
        
        int width;
        int height;
        int size;
        int blocksize;
};

typedef void (*Apply)(int col, int row, T array2b, void *elem, void *cl);


struct expandedcl {
        Apply apply;
        void *cl;
        T uarray2b;
        int blocksize;
};

T    UArray2b_new (int width, int height, int size, int blocksize) 
{
        assert(width >= 0 && height >= 0);
        assert(size > 0);
        assert(blocksize > 0);
        
        T array2b = malloc(sizeof(*array2b));
        assert(array2b != NULL);
        
        array2b->width = width;
        array2b->height = height;
        array2b->size = size;
        array2b->blocksize = blocksize;

        //UArray block = UArray_new(blocksize*blocksize, size);
        float fwidth = width;
        float fheight = height;
        

        array2b->array = UArray2_new(ceil(fwidth / blocksize), 
                                     ceil(fheight / blocksize), 
                                     sizeof(UArray_T));
                                     
        for (int i = 0; i < UArray2_width(array2b->array); i++)
        {
                for (int j = 0; j < UArray2_height(array2b->array); j++)
                {
                        UArray_T uarray = UArray_new(blocksize * blocksize, size);
                        //assert(uarray != NULL);
                        //uarray = UArray_new(blocksize * blocksize, size);
                        UArray_T *value = 
                                (UArray_T *) UArray2_at(array2b->array, i, j);
                        *value = uarray;
                }
                
        }
        

        return array2b;
}

T  UArray2b_new_64K_block(int width, int height, int size)
{
        assert(width >= 0 && height >= 0);
        assert(size > 0);

        int elemPerBlk = 1024 * 64 / size;
        int blocksize = ceil(sqrt(elemPerBlk));
        
        return UArray2b_new(width, height, size, blocksize);
}

void vFree(int col, int row, UArray2_T uarray2, void *element, void *cl)
{
        (void) cl;
        assert(uarray2 != NULL);
        assert(col >= 0 && col < UArray2_width(uarray2));
        assert(row >= 0 && row < UArray2_height(uarray2));

        UArray_free((UArray_T *)element);
        //free(*(UArray_T *)element);
}

void  UArray2b_free(T *array2b)
{
        assert(array2b != NULL);
        assert(*array2b != NULL);

        UArray2_map_row_major((*array2b)->array, vFree, NULL);
        UArray2_free(&((*array2b)->array));
        free(&((*array2b)->array));
        //free(*array2b);
}



int   UArray2b_width(T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}

int   UArray2b_height(T array2b)
{
        assert(array2b != NULL);
        return array2b->height;
}

int   UArray2b_size(T array2b)
{
        assert(array2b != NULL);
        return array2b->size;
}

int   UArray2b_blocksize(T array2b)
{
        assert(array2b != NULL);
        return array2b->blocksize;
}

/* return a pointer to the cell in the given column and row.
 * index out of range is a checked run-time error
 */
void *UArray2b_at(T array2b, int column, int row)
{
        assert(array2b != NULL);
        assert(column >= 0 && column < array2b->width);
        assert(row >= 0 && row < array2b->height);
        int blocksize = array2b->blocksize;
        
        UArray_T *inner = (UArray_T *)UArray2_at(array2b->array, 
                                        column / blocksize , row / blocksize);

        return UArray_at(*inner, 
                        blocksize * (column % blocksize) + (row % blocksize));
}

void Uapply(int col, int row, UArray2_T array2, void *elem, void *cl)
{

        assert(array2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        assert(col >= 0 && col < UArray2_width(array2));
        assert(row >= 0 && row < UArray2_height(array2));
        
        UArray_T *inner = (UArray_T *)elem;
        struct expandedcl *bundle = cl;
        Apply apply = bundle->apply;
        void *closure = bundle->cl;
        int blocksize = bundle->blocksize;
        T array2b = bundle->uarray2b;
        
        assert(apply != NULL);
        assert(blocksize > 0);
        assert(array2b != NULL);


        for (int i = 0; i < UArray_length(*inner); i++)
        {
                void *curr = UArray_at(*inner, i);
                int vcol = col * blocksize + (i / blocksize);
                int vrow = row * blocksize + (i % blocksize);
                //fprintf(stderr, "Current Val: %05u\n", *(int *)curr);
                if (vcol < array2b->width && vrow < array2b->height) {
                        apply(vcol, vrow, array2b, curr, closure);
                }

        }

        
}

/* visits every cell in one block before moving to another block */
void  UArray2b_map(T array2b, 
                void (*apply)(int col, int row, T array2b, void *elem, 
                                                                      void *cl),
                void *cl)
{
        assert(array2b != NULL);
        assert(apply != NULL);
        struct expandedcl *bundle = malloc(sizeof(*bundle));
        assert(bundle != NULL);
        bundle->apply = apply;
        bundle->cl = cl;
        bundle->blocksize = array2b->blocksize;
        bundle->uarray2b = array2b;
        
        UArray2_map_row_major(array2b->array, Uapply, bundle);

        free(bundle);
}

#undef T