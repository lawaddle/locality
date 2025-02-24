/**************************************************************
 *
 *                     uarray2b.c
 *
 *     Assignment: locality
 *     Authors:  Lawer Nyako (lnyako01) & Rigoberto Rodriguez-Anton (rrodri08)
 *     Date:    2-20-25
 *
 *     Summary: An implementation for a 2D version of Hanson's UArray data
 *              structure where elements are stored together in blocks. Clients
 *              can determine how large the blocks are or use the default size.
 *              It is built using a 2D Uarray where each element is block, that
 *              is a uarray of elements.
 *
 **************************************************************/

#include "uarray2.h"
#include "uarray2b.h"
#include <math.h>
#include <uarray.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#define T UArray2b_T

/*
 * Struct containing the the contents of the 2d blocked array. 
 */
struct T {
        /* the outer 2d array that will hold each block in the 2d array */
        UArray2_T array;
        
        int width;      /* width of 2d blocked array */
        int height;     /* height of 2d blocked array */
        int size;       /* the amount of bytes used by each element in the 
                         * array */
        int blocksize;  /* the width and height of a block in the 2d array */
};

/* 
 * typedef for the apply function, is used in the map function and as a 
 * part of expandedcl
 */
typedef void (*Apply)(int col, int row, T array2b, void *elem, void *cl);

/*
 * Struct to pass in multiple elements into the closure of the UArray2_map 
 * function inside the UArray2b_map function, so UArray2b_map can go through 
 * every element in a block, block by block.
 */
struct expandedcl {
        Apply apply;    /* apply function passed into UArray2b_map, so the 
        apply
                         * function can be used on each element in a block */
        void *cl;       /* original closure passed into UArray2b_map */
        T uarray2b;     /* 2d blocked array being mapped over */
        int blocksize;  /* blocksize of 2d blocked array being mapped over */
};

/********** UArray2b_new ********
 *
 * Creates and allocates space for a new 2D blocked UArray
 *
 * Parameters:
 *      int     width:          the number of columns in the 2D blocked UArray
 *      int     height:         the number of rows in the 2D blocked UArray
 *      int     size:           the amount of space the elements in the 2D
 *                              UArray will take up, each element will occupy
 *                              a size number bytes
 *      int     blocksize:      the width and height of each block in the 2D
 *                              blocked Uarray 
 *
 * Return: A pointer to the UArray2b structure that was created and malloc'd
 *
 * Expects: width and height to be non-negative (greater than or equal to 0),
 *          and for size and blocksize to positive (greater than 0)
 *      
 * Notes: 
 *      - Calls a CRE when width and height are less than 0
 *      - Calls a CRE when size is less than 1
 *      - Calls a CRE when blocksize is less than 1
 *      - Calls a CRE if fails to allocate memory for the UArray2b
 *      - Allocates memory for the UArray2b pointer, the Uarray2 inside it, 
 *      and the uarrays representing each block for each element of the 
 * Uarray2.
 *      User is responsible for calling UArray2b_free to free this memory.
 *      
 ************************/
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

        float fwidth = width;
        float fheight = height;

        /* to fit every element in a block, you need the width and height of 
         * the outer 2d array to be the ceilings of the width and height of the
         * total 2d blocked array divided by the blocksize*/
        array2b->array = UArray2_new(ceil(fwidth / blocksize), 
                                     ceil(fheight / blocksize), 
                                     sizeof(UArray_T));
                                     
        for (int i = 0; i < UArray2_width(array2b->array); i++)
        {
                for (int j = 0; j < UArray2_height(array2b->array); j++)
                {
                        /* making and populating a uarray for each block of the
                         * array2b */
                        UArray_T uarray = UArray_new(blocksize * blocksize, 
                                                                        size);
                        UArray_T *value = 
                                (UArray_T *) UArray2_at(array2b->array, i, j);
                        *value = uarray;
                }
                
        }
        
        return array2b;
}

/********** UArray2b_new_64K_block ********
 *
 * Creates and allocates space for a new 2D blocked UArray where each block is
 * 64 kB
 *
 * Parameters:
 *      int     width:          the number of columns in the 2D blocked UArray
 *      int     height:         the number of rows in the 2D blocked UArray
 *      int     size:           the amount of space the elements in the 2D
 *                              UArray will take up, each element will occupy
 *                              a size number bytes
 *
 * Return: A pointer to the UArray2b structure that was created and malloc'd
 *
 * Expects: width and height to be non-negative (greater than or equal to 0),
 *          and for size to positive (greater than 0)
 *      
 * Notes: 
 *      - Calls a CRE when width and height are less than 0
 *      - Calls a CRE when size is less than 1
 *      - Calls a CRE if fails to allocate memory for the UArray2b
 *      - Calls UArray2b_new that allocates memory for the UArray2b pointer, 
 *      the Uarray2 inside it, and the uarrays representing each block for each
 *      element of the Uarray2. User is responsible for calling UArray2b_free 
 *      to free this memory.
 *      
 ************************/
T  UArray2b_new_64K_block(int width, int height, int size)
{
        assert(width >= 0 && height >= 0);
        assert(size > 0);

        /* gets the number of bytes 64kb is and then divides it by the size 
         * of each element*/
        int elemPerBlk = 1024 * 64 / size;
        int blocksize = ceil(sqrt(elemPerBlk));
        
        return UArray2b_new(width, height, size, blocksize);
}

/********** vFree ********
 *
 * Frees the memory allocated the uarrays inside the Uarray2 for Uarray2b
 *
 * Parameters:
 *      int column:             current column index in 2d blocked Uarray  
 *      int row:                current row index in 2d blocked Uarray 
 *      UArray2_T uarray2:      the uarray2 that is being mapped through
 *      void *element:          the element, UArray_T, at each position in 
 *                              uarray2 
 *      void *cl:               closure for mapping function, will be voided
 * Return: n/a
 *
 * Expects: uarray2 and element to not be NULL; the column and row values not 
 *              to
 *          be out of bounds of the range for the inputted 2d blocked uarray
 *      
 * Notes: 
 *      CRE if:
 *              - uarray2 or element is null
 *              - row is less than 0 or greater or equal to the height of the 
 *               image (number of rows of the 2d blocked uarray)
 *              - column is less than 0 or greater or equal to the width of 
 * the 
 *               image (number of columns of the 2d blocked uarray)
 *      - Frees the uarray located at (col, row) of uarray2.
 ************************/
void vFree(int col, int row, UArray2_T uarray2, void *element, void *cl)
{
        (void) cl;
        assert(uarray2 != NULL);
        assert(element != NULL);
        assert(col >= 0 && col < UArray2_width(uarray2));
        assert(row >= 0 && row < UArray2_height(uarray2));

        UArray_free((UArray_T *)element);
}

/********** UArrary2b_free ********
 *
 * Frees the memory allocated for the 2D blocked UArray being pointed to
 *
 * Parameters:
 *      T *uarray2:     a pointer to the address of  UArray2b_T struct 
 *                              representing the UArray2b being accessed 
 *
 * Return: n/a
 *
 * Expects: uarray2 or *uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2 or *uarray2b is null
 *      - Frees the memory associated with the UArray2b including its pointer 
 *      the Uarray2 in it and the uarrays inside the Uarray2.
 ************************/
void  UArray2b_free(T *array2b)
{
        assert(array2b != NULL);
        assert(*array2b != NULL);

        /* maps through the uarray2 of blocks to free each internal uarray */
        UArray2_map_row_major((*array2b)->array, vFree, NULL);
        UArray2_free(&((*array2b)->array));
        free(&((*array2b)->array));
}


/********** UArrary2b_width ********
 *
 * Gets the width (number of columns) of the inputted uarray2b
 *
 * Parameters:
 *      T uarray2b:     a pointer to the UArray2b_T Struct representing
 *                      the UArray2b being accessed 
 *
 * Return: an integer representing the width (or number of columns) in the 2D
 *         blocked UArray
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *       - Calls CRE when uarray2 is null
 *      
 ************************/
int   UArray2b_width(T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}

/********** UArray2b_height ********
 *
 * Gets the height (number of rows) of the inputted uarray2b
 *
 * Parameters:
 *      T uarray2b:     a pointer to the UArray2b_T Struct representing
 *                      the UArray2b being accessed 
 *
 * Return: an integer representing the height (or number of rows) in the 2D
 *         blocked UArray
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *       - Calls CRE when uarray2 is null
 *      
 ************************/
int   UArray2b_height(T array2b)
{
        assert(array2b != NULL);
        return array2b->height;
}

/********** UArray2b_size ********
 *
 * Gets the memory size (number of bytes occupied) for an element in uarray2b
 *
 * Parameters:
 *      T uarray2b:     a pointer to the UArray2b_T Struct representing
 *                      the UArray2b being accessed 
 *
 * Return: the memory size (number of bytes occupied) for an element in 
 * uarray2b
 *
 * Expects: uarray2b to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2b is null
 *      
 ************************/
int   UArray2b_size(T array2b)
{
        assert(array2b != NULL);
        return array2b->size;
}

/********** UArray2b_blocksize ********
 *
 * Gets the blocksize (the width and height of each block) for array2b
 *
 * Parameters:
 *      T uarray2b:     a pointer to the UArray2b_T Struct representing
 *                      the UArray2b being accessed 
 *
 * Return: the blocksize for array2b
 *
 * Expects: uarray2b to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2b is null
 *      
 ************************/
int   UArray2b_blocksize(T array2b)
{
        assert(array2b != NULL);
        return array2b->blocksize;
}

/********** UArray2b_at ********
 *
 * Retrieves a pointer to the element stored at [column, row] in uarray2b
 *
 * Parameters:
 *      T       uarray2:        a pointer to the UArray2b_T Struct representing
 *                              the UArray2b being accessed
 *      int     column:         the column in the 2D blocked UArray being 
 *                              accessed
 *      int     row:            the row in the 2D blocked UArray being accessed
 *
 * Return: void * to value at (column, row) location in 2D blocked UArray
 *
 * Expects: column and row to not be greater than or equal to the width and
 *          height of uarray2b or less than 0; uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when the column and row passed in are greater than the
 *      bounds of uarray2b or if either is less than 0
 *      - Calls CRE when uarray2b is null
 *      
 ************************/
void *UArray2b_at(T array2b, int column, int row)
{
        assert(array2b != NULL);
        assert(column >= 0 && column < array2b->width);
        assert(row >= 0 && row < array2b->height);
        int blocksize = array2b->blocksize;
        
        /* gets the inner uarray where the block containing (column, row) is 
         * stored */
        UArray_T *inner = (UArray_T *)UArray2_at(array2b->array, 
                                        column / blocksize , row / blocksize);

        /* to get to the correct "column" in the block, then get to the 
         * correct "row" in the block*/
        return UArray_at(*inner, 
                        blocksize * (column % blocksize) + (row % blocksize));
}

/********** Uapply ********
 *
 * Takes the current block (a uarray) from UArray2b_map and maps through every
 * element in the block. Accounts for when blocks are not completely filled
 *
 * Parameters:
 *      int column:             current column index in 2d blocked Uarray  
 *      int row:                current row index in 2d blocked Uarray 
 *      UArray2_T uarray2:      the uarray2 that is being mapped through
 *      void *element:          the element, UArray_T, at each position in 
 *                              uarray2 
 *      void *cl:               closure for mapping function, should point to a
 *                              expandedcl struct
 *
 * Return: none
 *
 * Expects: array2, elem, and cl to not be null; the column and row values not 
 *          to be out of bounds of the range for the inputted 2d blocked 
 * uarray;
 *          the value of bit to be 0 or 1; apply and array2b in expandedcl 
 *          bundle are not null; blocksize in expandedcl is greater than  0
 *      
 * Notes: 
 *      CRE if:
 *              - array2, elem, or cl are null
 *              - row is less than 0 or greater or equal to the height of the 
 *               image (number of rows of the 2d blocked uarray)
 *              - column is less than 0 or greater or equal to the width of the
 *               image (number of columns of the 2d blocked uarray)
 *              - contents of expandedcl bundle are invalid:
 *                      - apply or array2b are null
 *                      - blocksize is less than 1
 *      
 ************************/
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
                if (vcol < array2b->width && vrow < array2b->height) {
                        apply(vcol, vrow, array2b, curr, closure);
                }

        }

        
}

/********** UArray2b_map ********
 *
 * Iterates through uarray2b in a block major fashion, iterating through 
 * every element in block before going to the next one
 *
 * Parameters:
 *      T       uarray2:        a pointer to the UArray2_T Struct representing
 *                              the UArray2 being accessed
 *
 *      void (*apply):          a function pointer that represents the function
 *                              to be called on the elements of uarray2b.
 *      Expects the function passed in to be void and to take in 5 parameters:
 *              int column:             the column index of the current element
 *              int row:                the row index of the current element
 *              T uarray2b:             a pointer to the uarray2b being 
 *                                      traversed
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure 
 * that
 *                                      can be updated during a given traversal
 *      
 *      void *cl:               a void pointer representing the closure which is
 *                              a variable that can be updated as one traverses
 *                              through the uarray2b
 *
 * Return: none
 *
 * Expects: uarray2b and apply to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2b is null
 *      - Calls CRE when apply is null
 *      - Allocates and frees memory for expandedcl struct
 *      
 ************************/
void  UArray2b_map(T array2b, 
                void (*apply)(int col, int row, T array2b, void *elem, 
                                                                void *cl),
                void *cl)
{
        assert(array2b != NULL);
        assert(apply != NULL);
        /**
         * Values needed to pass into the UArray2_map_row_major closure, so
         *  each
         * block, and thus each element in a block would have access to these
         * values:
         *      - The apply function, so it actually runs on each element
         *      - the closure, if the client is passing in their own closure
         *      that needs to get accessed by the elements of Uarrayb
         *      - the blocksize of the array2b, to find the row/col of the 
         *      Uarray2b using row/col of the outer Uarray2 and the current 
         *      index of the inner uarray
         *      - the array2b
         */
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