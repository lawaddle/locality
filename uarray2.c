/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: iii
 *     Authors:  Hannah Fiarman (hfiarm01), Lawer Nyako (lnyako01) 
 *     Date:    2-10-25
 *
 *     Summary: The implementation for a 2D version of Hanson's UArray data 
 *              structure and its relevent functions. It is built upon a single
 *              UArray that stores the data for a 2D implementation.
 *
 **************************************************************/

#include "uarray2.h"

#define T UArray2_T

struct T {
        int width;
        int height;
        int size;

        UArray_T array;
};

/********** UArrary2_new ********
 *
 * Creates and allocates space for a new 2D UArray
 *
 * Parameters:
 *      int     width:          the number of columns in the 2D UArray
 *      int     height:         the number of rows in the 2D UArray
 *      int     size:           the amount of space the elements in the 2D
 *                              UArray will take up, each element will occupy
 *                              a size number bytes
 *
 * Return: A pointer to the UArray2 that was created and malloc'd
 *
 * Expects: width and height to be non-negative (greater than or equal to 0),
 *          and for size to positive (greater than 0)
 *      
 * Notes: 
 *      - Calls a CRE when width and height are less than 0
 *      - Calls a CRE when size is less than 1
 *      - Calls a CRE if fails to allocate memory for the UArray2
 *      - Allocates memory for the UArray2 pointer and the subsequent UArray it
 *      holds. User is responsible for calling UArray2_free to free this memory.
 *      
 ************************/
T UArray2_new(int width, int height, int size) 
{
        assert(width >= 0 && height >= 0);
        assert(size > 0);

        T uarray2 = malloc(sizeof(*uarray2));
        assert(uarray2 != NULL);

        uarray2->width = width;
        uarray2->height = height;
        uarray2->size = size;

        /* the number of elements of the uarray representing the uarray2 is 
        the width of 2d uarray times its height */
        uarray2->array = UArray_new((width * height), size);

        return uarray2;
}

/********** UArrary2_free ********
 *
 * Frees the memory allocated for the 2D UArray being pointed to
 *
 * Parameters:
 *      T *uarray2:     a pointer to the address of  UArray2_T struct 
 *                              representing the UArray2 being accessed 
 *
 * Return: n/a
 *
 * Expects: uarray2 or *uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2 or *uarray2 is null
 *      - Frees the memory associated with the UArray2 including its pointer and
 *      the UArray within it.
 *      
 ************************/
void UArray2_free(T *uarray2) 
{
        assert(uarray2 != NULL);
        assert(*uarray2 != NULL);

        UArray_free(&((*uarray2)->array));
        free(*uarray2);
}

/********** UArrary2_width ********
 *
 * Gets the width (number of columns) of the inputted uarray2
 *
 * Parameters:
 *      T uarray2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: an integer representing the width (or number of columns) in the 2D
 *         UArray
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *       - Calls CRE when uarray2 is null
 *      
 ************************/
int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/********** UArrary2_height ********
 *
 * Gets the height (number of rows) of the inputted uarray2
 *
 * Parameters:
 *      T uarray2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: an integer representing the height (or number of rows) in the 2D
 *         UArray
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/********** UArrary2_size ********
 *
 * Gets the memory size (number of bytes occupied) for an element in uarray2
 *
 * Parameters:
 *      T uarray2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: the memory size (number of bytes occupied) for an element in uarray2 
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

/********** UArrary2_at ********
 *
 * Retrieves a pointer to the element stored at [column, row] in uarray2
 *
 * Parameters:
 *      T       uarray2:        a pointer to the UArray2_T Struct representing
 *                              the UArray2 being accessed
 *      int     column:         the column in the 2D UArray being accessed
 *      int     row:            the row in the 2D UArray being accessed
 *
 * Return: void * to value at (column, row) location in 2D UArray
 *
 * Expects: column and row to not be greater than or equal to the width and
 *          height of uarray2 or less than 0; uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when the column and row passed in are greater than the
 *      bounds of uarray2 or if either is less than 0
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
void *UArray2_at(T uarray2, int column, int row)
{
        assert(uarray2 != NULL);
        assert(column >= 0 && column < uarray2->width);
        assert(row >= 0 && row < uarray2->height);
        
        /* To get location of an element in the 2d uarray, get to the start
         * of the row you are looking for (by multiplying the inputted row by
         * the width) then go forward to the column you want (adding the 
         * inputted column to the row * width) */
        return UArray_at(uarray2->array, (row) * (uarray2->width) + (column)); 
}

/********** UArrary2_map_col_major ********
 *
 * Iterates through uarray2 in a column major fashion, calling the provided 
 * apply function on each element going through the uarray2 column by column
 *
 * Parameters:
 *      T       uarray2:        a pointer to the UArray2_T Struct representing
 *                              the UArray2 being accessed
 *
 *      void (*apply):          a function pointer that represents the function
 *                              to be called on the elements of uarray2.
 *      Expects the function passed in to be void and to take in 5 parameters:
 *              int column:             the column index of the current element
 *              int row:                the row index of the current element
 *              T uarray2:              a pointer to the uarray2 being traversed
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure that
 *                                      can be updated during a given traversal
 *      
 *      void *cl:               a void pointer representing the closure which is
 *                              a variable that can be updated as one traverses
 *                              through the uarray2
 *
 * Return: none
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
void UArray2_map_col_major(T uarray2, void (*apply)(int column, int row,
                        T uarray2, void *element, void *cl), void *cl)
{
        assert(uarray2 != NULL);
        assert(apply != NULL);

        /* to map the 2d uarray in a column major fashion go across the height 
         * of the array then go to the next column, repeat this until you've 
         * mapped the whole array */
        for (int i = 0; i < uarray2->width; i++) {
                for (int j = 0; j < uarray2->height; j++) {
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
                }
        }
}

/********** UArrary2_map_row_major ********
 *
 * Iterates through uarray2 in a row major fashion, calling the provided apply
 * function on each element going through the uarray2 row by row
 *
 * Parameters:
 *      T       uarray2:        a pointer to the UArray2_T Struct representing
 *                              the UArray2 being accessed
 *
 *      void (*apply):          a function pointer that represents the function
 *                              to be called on the elements of uarray2.
 *      Expects the function passed in to be void and to take in 5 parameters:
 *              int column:             the column index of the current element
 *              int row:                the row index of the current element
 *              T uarray2:              a pointer to the uarray2 being traversed
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure that
 *                                      can be updated during a given traversal
 *      
 *      void *cl:               a void pointer representing the closure which is
 *                              a variable that can be updated as one traverses
 *                              through the uarray2
 *
 * Return: none
 *
 * Expects: uarray2 to not be NULL and that the apply function has parameters
 *          for the column and row of the 2D array
 *      
 * Notes: 
 *      - Calls CRE when uarray2 is null
 *      - Calls CRE when apply is null
 *      
 ************************/
void UArray2_map_row_major(T uarray2, void (*apply)(int column, int row,
                        T uarray2, void *element, void *cl), void *cl)
{
        assert(uarray2 != NULL);
        assert(apply != NULL);
        
        /* to map the 2d uarray in a row major fashion go across the width of 
         * the array then go to the next row, repeat this until you've mapped 
         * the whole array */
        for (int i = 0; i < uarray2->height; i++) {
                for (int j = 0; j < uarray2->width; j++) {
                        apply(j, i, uarray2, UArray2_at(uarray2, j, i), cl);
                }
        }
}

#undef T