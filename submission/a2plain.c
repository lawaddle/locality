/**************************************************************
 *
 *                     a2plain.c
 *
 *     Assignment: locality
 *     Authors:  Lawer Nyako (lnyako01) & Rigoberto Rodriguez-Anton (rrodri08)
 *     Date:    2-20-25
 *
 *     Summary: A subclass for A2Methods_T virtual class that allows methods to
 *      be done with plain uarray2 data structures. Putting functions in this
 *      manner allows for encapsulation and polymorphism.
 *
 **************************************************************/

#include <string.h>

#include <a2plain.h>
#include "uarray2.h"

/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/
typedef A2Methods_UArray2 A2;   /* private abbreviation */

/********** new ********
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
 *      (all of these are done in the UArray2_new function called)
 *      - Calls a CRE when width and height are less than 0
 *      - Calls a CRE when size is less than 1
 *      - Calls a CRE if fails to allocate memory for the UArray2
 *      - Allocates memory for the UArray2 pointer and the subsequent UArray it
 *      holds. User is responsible for calling UArray2_free to free this memory
 *      
 ************************/
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/********** new_with_blocksize ********
 *
 * Creates and allocates space for a new 2D UArray with a blocksize
 *
 * Parameters:
 *      int     width:          the number of columns in the 2D UArray
 *      int     height:         the number of rows in the 2D UArray
 *      int     size:           the amount of space the elements in the 2D
 *                              UArray will take up, each element will occupy
 *                              a size number bytes
 *      int     blocksize:      the width and height of each block in the 2D
 *                              blocked Uarray (doesn't apply here)
 *
 * Return: A pointer to the UArray2 that was created and malloc'd
 *
 * Expects: width and height to be non-negative (greater than or equal to 0),
 *          and for size to positive (greater than 0)
 *      
 * Notes: 
 *      - because this is called on a plain Uarray2, it does not matter what 
 *      the inputted blocksize is as, plain uarray2s don't have blocks or 
 *      blocksizes
 *      (all of these are done in the UArray2_new function called)
 *      - Calls a CRE when width and height are less than 0
 *      - Calls a CRE when size is less than 1
 *      - Calls a CRE if fails to allocate memory for the UArray2
 *      - Allocates memory for the UArray2 pointer and the subsequent UArray it
 *      holds. User is responsible for calling UArray2_free to free this memory
 *      
 ************************/
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void) blocksize;
        return new(width, height, size);
}

/********** a2free ********
 *
 * Frees the memory allocated for the 2D UArray being pointed to
 *
 * Parameters:
 *      A2 *uarray2:     a pointer to the address of  UArray2_T struct 
 *                              representing the UArray2 being accessed 
 *
 * Return: n/a
 *
 * Expects: uarray2 or *uarray2 to not be NULL
 *      
 * Notes: 
 *      (done in the UArray2_free function called)
 *      - Calls CRE when uarray2 or *uarray2 is null
 *      - Frees the memory associated with the UArray2 including its pointer 
 * and
 *      the UArray within it.
 *      
 ************************/
static void a2free(A2 * array2p)
{
        UArray2_free((UArray2_T *) array2p);
}

/********** width ********
 *
 * Gets the width (number of columns) of the inputted array2
 *
 * Parameters:
 *      A2 uarray2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: an integer representing the width (or number of columns) in the 2D
 *         UArray
 * 
 * Expects: array2 to not be NULL
 *      
 * Notes: 
 *      (done in the UArray2_size function called)
 *      - Calls CRE when array2 is null
 *      
 ************************/
static int width(A2 array2)
{
        return UArray2_width(array2);
}

/********** height ********
 *
 * Gets the height (number of rows) of the inputted array2
 *
 * Parameters:
 *      A2 uarray2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: an integer representing the height (or number of rows) in the 2D
 *         UArray
 * 
 * Expects: array2 to not be NULL
 *      
 * Notes: 
 *      (done in the UArray2_size function called)
 *      - Calls CRE when array2 is null
 *      
 ************************/
static int height(A2 array2)
{
        return UArray2_height(array2);
}

/********** size ********
 *
 * Gets the memory size (number of bytes occupied) for an element in array2
 *
 * Parameters:
 *      A2 uarray2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: the memory size (number of bytes occupied) for an element in uarray2
 *
 * Expects: array2 to not be NULL
 *      
 * Notes: 
 *      (done in the UArray2_size function called)
 *      - Calls CRE when array2 is null
 *      
 ************************/
static int size(A2 array2)
{
        return UArray2_size(array2);
}

/********** blocksize ********
 *
 * Gets the blocksize (the width and height of each block) for array2
 *
 * Parameters:
 *      A2 array2:      a pointer to the UArray2_T Struct representing
 *                      the UArray2 being accessed 
 *
 * Return: the memory size (number of bytes occupied) for an element in uarray2 
 *
 * Expects: n/a
 *      
 * Notes: 
 *      - Because this function will be called on uarray2 plains, these data
 *      structures don't have blocksizes so the blocksize will be 0 
 *      
 ************************/
static int blocksize(A2 array2)
{
        (void) array2;
        return 0;
}

/********** at ********
 *
 * Retrieves a pointer to the element stored at [column, row] in array2
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
 *      (are done inside the called UArray2_at function)
 *      - Calls CRE when the column and row passed in are greater than the
 *      bounds of uarray2 or if either is less than 0
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
static A2Methods_Object *at(A2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}

typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

/********** map_col_major ********
 *
 * Iterates through uarray2 in a row major fashion, calling the provided apply
 * function on each element going through the uarray2 row by row
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2:      a pointer to the UArray2_T Struct 
 *                                      representing the UArray2 being accessed
 *
 *      A2Methods_applyfun (*apply):    a function pointer that represents the 
 *                                      function to be called on the elements 
 * of
 *                                      uarray2. Since it is of type applyfun
 *                                      the compiler will know to check that 
 *                                      the function passed as the apply 
 *                                      function has the necessary parameters
 *      Expects the function passed in to be void and to take in 5 parameters:
 *              int column:             the column index of the current element
 *              int row:                the row index of the current element
 *              T uarray2:              a pointer to the uarray2 being 
 * traversed
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure 
 * that
 *                                      can be updated during a given traversal
 *      
 *      void *cl:               a void pointer representing the closure which 
 * is
 *                              a variable that can be updated as one traverses
 *                              through the uarray2
 *
 * Return: none
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *      (is done in the UArray2_map_row_major function called)
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (applyfun*)apply, cl);
}

/********** map_col_major ********
 *
 * Iterates through uarray2 in a column major fashion, calling the provided 
 * apply function on each element going through the uarray2 column by column
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2:      a pointer to the UArray2_T Struct 
 *                                      representing the UArray2 being accessed
 *
 *      A2Methods_applyfun (*apply):    a function pointer that represents the 
 *                                      function to be called on the elements of
 *                                      uarray2. Since it is of type applyfun
 *                                      the compiler will know to check that 
 *                                      the function passed as the apply 
 *                                      function has the necessary parameters
 *      Expects the function passed in to be void and to take in 5 parameters:
 *              int column:             the column index of the current 
 * element
 *              int row:                the row index of the current element
 *              T uarray2:              a pointer to the uarray2 being 
 * traversed
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure 
 * that
 *                                      can be updated during a given traversal
 *      
 *      void *cl:               a void pointer representing the closure which 
 * is
 *                              a variable that can be updated as one traverses
 *                              through the uarray2
 *
 * Return: none
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *      (is done in the UArray2_map_col_major function called)
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (applyfun*)apply, cl);
}

/* closure for apply and cl for small mapping where you give the apply 
 * function only access to the inputted closure and the current element*/
struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};


static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}

/********** small_map_row_major ********
 *
 * Iterates through uarray2 in a row major fashion, calling the provided 
 * apply function on each element going through the uarray2 row by row
 * but the apply function only has access to the current element and the 
 * closure
 *
 * Parameters:
 *      A2Methods_UArray2 a2:      a pointer to the UArray2_T Struct 
 *                                      representing the UArray2 being accessed
 *
 *      A2Methods_smallapplyfun (*apply):    
 *                                      a function pointer that represents the 
 *                                      function to be called on the elements 
 * of
 *                                      uarray2. Since it is of type 
 *                                      smallapplyfun
 *                                      the compiler will know to check that 
 *                                      the function passed as the apply 
 *                                      function has the necessary parameters
 *      Expects the function passed in to be void and to take in 2 parameters:
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure 
 * that
 *                                      can be updated during a given traversal
 *      
 *      void *cl:               a void pointer representing the closure which 
 * is
 *                              a variable that can be updated as one traverses
 *                              through the uarray2
 *
 * Return: none
 *
 * Expects: uarray2 to not be NULL
 *      
 * Notes: 
 *      (is done in the small_map_row_major function called)
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/********** small_map_row_major ********
 *
 * Iterates through uarray2 in a column major fashion, calling the provided 
 * apply function on each element going through the uarray2 column by column
 * but the apply function only has access to the current element and the 
 * closure
 *
 * Parameters:
 *      A2Methods_UArray2 a2:      a pointer to the UArray2_T Struct 
 *                                      representing the UArray2 being accessed
 *
 *      A2Methods_smallapplyfun (*apply):    
 *                                      a function pointer that represents the 
 *                                      function to be called on the elements 
 *                                      of
 *                                      uarray2. Since it is of type 
 *                                      smallapplyfun
 *                                      the compiler will know to check that 
 *                                      the function passed as the apply 
 *                                      function has the necessary parameters
 *      Expects the function passed in to be void and to take in 2 parameters:
 *              void *element:          the current element of the iteration
 *              void *cl:               a variable representing the closure 
 * that
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
 *      (is done in the UArray2_map_col_major function called)
 *      - Calls CRE when uarray2 is null
 *      
 ************************/
static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/**
 * This A2Methods_T struct holds the methods allowing for the methods in this 
 * file to act as the methods of a subclass of the virtual class A2Methods_T.
 * The map_block_major and small_map_block_major functions are not implemented 
 * when using plain Uarray2s so are null. We set map_default and 
 * small_map_default to map_row_major and small_map_row_major respectively 
 * because we determined that the high spatial locality of traversing row_major
 * would mean that generally row_major should be faster than col_major. The
 * remaining elements in this struct are the function pointers of the above 
 * functions in this file.
 */
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,
        map_col_major,
        NULL,                   /* map_block_major */
        map_row_major,          /* map_default */
        small_map_row_major,
        small_map_col_major,
        NULL,                   /* small_map_block_major */
        small_map_row_major,    /* small_map_default */
};

/* finally the payoff: here is the exported pointer to the struct */

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;