/**************************************************************
 *
 *                     ppmtrans.c
 *
 *     Assignment: locality
 *     Authors:  Lawer Nyako (lnyako01) & Rigoberto Rodriguez-Anton (rrodri08)
 *     Date:    2-20-25
 *
 *     Summary: Takes a ppm image and transforms it based on command line 
 *      input. The user can decide if the transformation should be done using
 *      row major, column major, or block major mapping allowing for different 
 *      locality benefits.
 *
 **************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

typedef A2Methods_UArray2 A2;


struct mappingCl {
        A2 newMap;
        A2Methods_T methods;

};

/* Definition of transformation options used by main and transform */
#define ZERO 0
#define NINETY 90
#define ONE_EIGHTY 180
#define TWO_SEVENTY 270
#define HORIZONTAL 1
#define VERTICAL 2
#define TRANSPOSE 3

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (false)

/********** usage ********
 *
 * Purpose:
 *      Displays the correct usage of the program and exits with an error 
 *      status. This function provides guidance on how to use the command-line 
 *      arguments expected by the program. 
 *
 * Parameters:
 *      const char *progname:     The name of the program.
 *
 * Return: 
 *      N/A
 *
 * Notes:
 *      - The usage message includes options for rotation angles, mapping 
 *        methods, and optional timing file output.
 *      - Calls exit(1) to terminate the program upon invocation, 
 *        indicating an error.
 *
 ************************/
static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s ([-rotate <angle>] OR [-transpose] OR "
                        "[-flip <vertical,horizontal>]) "
                        "[-{row,col,block}-major] "
                        "[-time time_file] "
                        "[filename]\n",
                        progname);
        exit(1);
}

/********** apply90 ********
 *
 * Rotates an image represented by a UArray2 structure by 90 degrees.
 *
 * Parameters:
 *      int     i:              The current row in the original image.
 *      int     j:              The current column in the original image.
 *      A2      array2:         The UArray2 representing the original 
 *                              image.
 *      void    *elem:          A pointer to the current pixel's RGB value.
 *      void    *cl:            A pointer to a structure containing necessary 
 *                              context (e.g., methods and newMap).
 *
 * Return: 
 *      N/A
 *
 * Preconditions:
 *      - array2 must not be NULL.
 *      - cl must not be NULL.
 *      - elem must not be NULL.
 *      - The contents of the cl struct must not be NULL.
 *      - i must be a non-negative integer within the width of array2.
 *      - j must be a non-negative integer within the height of array2.
 *
 * Notes: 
 *      - Calls a CRE if any of the above preconditions are 
 *        violated.
 *      - Assumes the memory for newMap has been allocated and initialized 
 *        correctly before this function is called.
 *      - The user is responsible for calling UArray2b_free to free any 
 *        allocated memory associated with the UArray2 structure.
 *
 ************************/
void apply90(int i, int j, A2 array2, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);

        
        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2));
        assert(j >= 0 && j < methods->height(array2));

        /* Rotate image 90 degrees clockwise. */
        struct Pnm_rgb *rgb = elem;
        int height = methods->height(array2);
        *(struct Pnm_rgb *)methods->at(newMap, height - j - 1, i) 
                                                      = *(struct Pnm_rgb *)rgb;
}

/********** apply180 ********
 *
 * Rotates an image represented by a UArray2 structure by 180 degrees.
 *
 * Parameters:
 *      int     i:              The current row in the original image.
 *      int     j:              The current column in the original image.
 *      A2      array2:         The UArray2 representing the original 
 *                              image.
 *      void    *elem:          A pointer to the current pixel's RGB value.
 *      void    *cl:            A pointer to a structure containing necessary 
 *                              context (e.g., methods and newMap).
 *
 * Return: 
 *      N/A
 *
 * Preconditions:
 *      - array2 must not be NULL.
 *      - cl must not be NULL.
 *      - elem must not be NULL.
 *      - The contents of the cl struct must not be NULL.
 *      - i must be a non-negative integer within the width of array2.
 *      - j must be a non-negative integer within the height of array2.
 *
 * Notes: 
 *      - Calls a CRE if any of the above preconditions are 
 *        violated.
 *      - Assumes the memory for newMap has been allocated and initialized 
 *        correctly before this function is called.
 *      - The user is responsible for calling UArray2b_free to free any 
 *        allocated memory associated with the UArray2 structure.
 *
 ************************/
void apply180(int i, int j, A2 array2, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2));
        assert(j >= 0 && j < methods->height(array2));

        /* Rotate image 180 degrees. */
        struct Pnm_rgb *rgb = elem;
        int width = methods->width(array2);
        int height = methods->height(array2);
        *(struct Pnm_rgb *)methods->at(newMap, width - i - 1, height - j - 1)
                                                      = *(struct Pnm_rgb *)rgb;
}

/********** apply270 ********
 *
 * Rotates an image represented by a UArray2 structure by 270 degrees.
 *
 * Parameters:
 *      int     i:              The current row in the original image.
 *      int     j:              The current column in the original image.
 *      A2      array2:         The UArray2 representing the original 
 *                              image.
 *      void    *elem:          A pointer to the current pixel's RGB value.
 *      void    *cl:            A pointer to a structure containing necessary 
 *                              context (e.g., methods and newMap).
 *
 * Return: 
 *      N/A
 *
 * Preconditions:
 *      - array2 must not be NULL.
 *      - cl must not be NULL.
 *      - elem must not be NULL.
 *      - The contents of the cl struct must not be NULL.
 *      - i must be a non-negative integer within the width of array2.
 *      - j must be a non-negative integer within the height of array2.
 *
 * Notes: 
 *      - Calls a CRE if any of the above preconditions are 
 *        violated.
 *      - Assumes the memory for newMap has been allocated and initialized 
 *        correctly before this function is called.
 *      - The user is responsible for calling UArray2b_free to free any 
 *        allocated memory associated with the UArray2 structure.
 *
 ************************/
void apply270(int i, int j, A2 array2, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2));
        assert(j >= 0 && j < methods->height(array2));
        
        /* Rotate image 270 degrees clockwise (or 90 ccw). */
        int width = methods->width(array2);
        struct Pnm_rgb *rgb = elem;
        *(struct Pnm_rgb *)methods->at(newMap, j, width - i - 1)
                                                      = *(struct Pnm_rgb *)rgb;
}

/********** applyTranspose ********
 *
 * Transposes and image represented by a UArray2 structure.
 *
 * Parameters:
 *      int     i:              The current row in the original image.
 *      int     j:              The current column in the original image.
 *      A2      array2:         The UArray2 representing the original 
 *                              image.
 *      void    *elem:          A pointer to the current pixel's RGB value.
 *      void    *cl:            A pointer to a structure containing necessary 
 *                              context (e.g., methods and newMap).
 *
 * Return: 
 *      N/A
 *
 * Preconditions:
 *      - array2 must not be NULL.
 *      - cl must not be NULL.
 *      - elem must not be NULL.
 *      - The contents of the cl struct must not be NULL.
 *      - i must be a non-negative integer within the width of array2.
 *      - j must be a non-negative integer within the height of array2.
 *
 * Notes: 
 *      - Calls a CRE if any of the above preconditions are 
 *        violated.
 *      - Assumes the memory for newMap has been allocated and initialized 
 *        correctly before this function is called.
 *      - The user is responsible for calling UArray2b_free to free any 
 *        allocated memory associated with the UArray2 structure.
 *
 ************************/
void applyTranspose(int i, int j, A2 array2, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);
        assert(elem != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2));
        assert(j >= 0 && j < methods->height(array2));
        
        /* Transpose image (across UL-to-LR axis). */
        struct Pnm_rgb *rgb = elem;
        *(struct Pnm_rgb *)methods->at(newMap, j, i) = *(struct Pnm_rgb *)rgb;
}

/********** applyHorizontal ********
 *
 * Flips an image represented by a UArray2 structure horizontally.
 *
 * Parameters:
 *      int     i:              The current row in the original image.
 *      int     j:              The current column in the original image.
 *      A2      array2:         The UArray2 representing the original 
 *                              image.
 *      void    *elem:          A pointer to the current pixel's RGB value.
 *      void    *cl:            A pointer to a structure containing necessary 
 *                              context (e.g., methods and newMap).
 *
 * Return: 
 *      N/A
 *
 * Preconditions:
 *      - array2 must not be NULL.
 *      - cl must not be NULL.
 *      - elem must not be NULL.
 *      - The contents of the cl struct must not be NULL.
 *      - i must be a non-negative integer within the width of array2.
 *      - j must be a non-negative integer within the height of array2.
 *
 * Notes: 
 *      - Calls a CRE if any of the above preconditions are 
 *        violated.
 *      - Assumes the memory for newMap has been allocated and initialized 
 *        correctly before this function is called.
 *      - The user is responsible for calling UArray2b_free to free any 
 *        allocated memory associated with the UArray2 structure.
 *
 ************************/
void applyHorizontal(int i, int j, A2 array2, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2));
        assert(j >= 0 && j < methods->height(array2));

        /* Mirror image horizontally (lef-right). */
        struct Pnm_rgb *rgb = elem;
        int height = methods->height(array2);
        *(struct Pnm_rgb *)methods->at(newMap, i, height - j - 1)
                                                      = *(struct Pnm_rgb *)rgb;
}

/********** applyVertical ********
 *
 * Flips an image represented by a UArray2 structure horizontally.
 *
 * Parameters:
 *      int     i:              The current row in the original image.
 *      int     j:              The current column in the original image.
 *      A2      array2:         The UArray2 representing the original 
 *                              image.
 *      void    *elem:          A pointer to the current pixel's RGB value.
 *      void    *cl:            A pointer to a structure containing necessary 
 *                              context (e.g., methods and newMap).
 *
 * Return: 
 *      N/A
 *
 * Preconditions:
 *      - array2 must not be NULL.
 *      - cl must not be NULL.
 *      - elem must not be NULL.
 *      - The contents of the cl struct must not be NULL.
 *      - i must be a non-negative integer within the width of array2.
 *      - j must be a non-negative integer within the height of array2.
 *
 * Notes: 
 *      - Calls a CRE if any of the above preconditions are 
 *        violated.
 *      - Assumes the memory for newMap has been allocated and initialized 
 *        correctly before this function is called.
 *      - The user is responsible for calling UArray2b_free to free any 
 *        allocated memory associated with the UArray2 structure.
 *
 ************************/
void applyVertical(int i, int j, A2 array2, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2));
        assert(j >= 0 && j < methods->height(array2));

        /* Mirror image vertically (top-bottom). */
        struct Pnm_rgb *rgb = elem;
        int width = methods->width(array2);
        *(struct Pnm_rgb *)methods->at(newMap, width - i - 1, j)
                                                      = *(struct Pnm_rgb *)rgb;
}

/********** transform ********
 *
 *      The transform function applies various transformations (e.g., 
 *      rotation, transposition, horizontal and vertical flipping) to a Pnm_ppm
 *      image. 
 *
 * Parameters:
 *      Pnm_ppm                 ppmMap:         The Pnm_ppm structure 
 *                                              representing the image.
 *      int                     transformation: The type of transformation to 
 *                                              apply (e.g., NINETY, ONE_EIGHTY,
                                                etc.).
 *      A2Methods_mapfun        *map:           A pointer to the mapping 
 *                                              function for applying 
 *                                              transformations.
 *      A2Methods_T             methods:        A structure containing methods 
 *                                              for manipulating the UArray2.
 *
 * Return: 
 *      Pnm_ppm: A pointer to the transformed Pnm_ppm structure.
 *
 * Preconditions:
 *      - ppmMap must not be NULL.
 *      - map must not be NULL, and the function it points to must not be 
 *        NULL.
 *      - methods must not be NULL.
 *
 * Notes: 
 *      - If the transformation is ZERO, the original ppmMap is returned 
 *        without modification.
 *      - Memory for the new UArray2 is allocated and initialized based on the 
 *        dimensions of the original image and the type of transformation to be
 *        performed.
 *      - The user is responsible for freeing the memory of the original 
 *        ppmMap after use.
 *
 ************************/
Pnm_ppm transform(Pnm_ppm ppmMap, int transformation, A2Methods_mapfun *map, 
                                                           A2Methods_T methods)
{
        assert(methods != NULL);
        assert(map != NULL);
        assert(*map != NULL);
        assert(ppmMap != NULL);
        
        /* 
         * Separately handles a rotation of 0 degrees by simply returning the
         * original image.
         */
        if (transformation == ZERO) {
                return ppmMap;
        }
        int newHeight = methods->height(ppmMap->pixels);
        int newWidth = methods->width(ppmMap->pixels);
        /* 
         * Changes the dimensions of the destination array if required based on
         * the type of transformation. 
         */
        if (transformation == NINETY || 
            transformation == TWO_SEVENTY || 
            transformation == TRANSPOSE) {
                newHeight = methods->width(ppmMap->pixels);
                newWidth = methods->height(ppmMap->pixels);
        }
        
        A2 newMap = methods->new(newWidth, newHeight, sizeof(struct Pnm_rgb));
        struct mappingCl bundle = {newMap, methods};

        /* These if statements determine which apply function will be used. */
        if (transformation == NINETY) {
                map(ppmMap->pixels, apply90, &bundle);
        } else if (transformation == ONE_EIGHTY) {
                map(ppmMap->pixels, apply180, &bundle);
        } else if (transformation == TWO_SEVENTY) {
                map(ppmMap->pixels, apply270, &bundle);
        } else if (transformation == TRANSPOSE) {
                map(ppmMap->pixels, applyTranspose, &bundle);
        } else if (transformation == HORIZONTAL) {
                map(ppmMap->pixels, applyHorizontal, &bundle);
        } else if (transformation == VERTICAL) {
                map(ppmMap->pixels, applyVertical, &bundle);
        }
        
        methods->free(&(ppmMap->pixels));
        ppmMap->width = newWidth;
        ppmMap->height = newHeight;
        ppmMap->pixels = newMap; 
        return ppmMap;
}

/********** main ********
 *
 *      main manages the inputs and outputs of the ppmtrans program, parsing
 *      flags and parameters, and executing accordingly
 *
 * Parameters:
 *      int   argc:     The number of command-line arguments.
 *      char *argv[]:   An array of command-line argument strings.
 *
 * Return: 
 *      int: Returns EXIT_SUCCESS (0) on successful execution. Returns 
 *      EXIT_FAILURE (1) if the usage was invalid.
 *
 * Preconditions:
 *      - Command-line arguments must be properly formatted.
 *      - Input file must be a valid Pnm_ppm file if specified.
 *
 * Notes:
 *      - 
 *      - Handles various command-line options for transformations and methods.
 *      - Ensures memory is freed for the original Pnm_ppm structure.
 *      - If the output file is not specified, it uses standard input.
 *      - Logs execution time if a timing file is provided.
 *
 ************************/
int main(int argc, char *argv[])
{
        char *time_file_name = NULL;
        char *out_file_name = NULL;

        int   transformation       = 0;
        int   i;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        transformation = strtol(argv[++i], &endptr, 10);
                        if (!(transformation == ZERO || 
                              transformation == NINETY ||
                              transformation == ONE_EIGHTY || 
                              transformation == TWO_SEVENTY)) {
                                fprintf(stderr, 
                                        "rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-transpose") == 0) {
                        transformation = TRANSPOSE;
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if (!(i + 1 < argc)) {      /* no flip chosen */
                                usage(argv[0]);
                        } 
                        char *flip = argv[++i];
                        if (strcmp(flip, "horizontal") == 0) {
                                transformation = HORIZONTAL;
                        } else if (strcmp(flip, "vertical") == 0) {
                                transformation = VERTICAL;
                        } else {
                                fprintf(stderr, 
                                  "flip must be 'horizontal' or 'vertical'\n");
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        if (!(i + 1 < argc)) {      /* no time file */
                                usage(argv[0]);
                        }
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (*argv[i] != '-'){
                        out_file_name = argv[i];
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        FILE *fp;
        if (out_file_name == NULL) {
                fp = stdin;
        } else {
                fp = fopen(out_file_name, "r");
                assert(fp != NULL);
        }

        Pnm_ppm ppmMap = Pnm_ppmread(fp, methods);
        fclose(fp);

        /* 
         * start timer after ppmMap is made and the image is read and before 
         * transforming happens 
         */
        CPUTime_T timer = CPUTime_New();
        CPUTime_Start(timer);
        Pnm_ppm transformed = transform(ppmMap, transformation, map, methods);
        double cputime = CPUTime_Stop(timer);
        /* 
         * stop timer after transformation and before writing the 
         * transformed ppm to stdout 
         */
        Pnm_ppmwrite(stdout, transformed);
        double pixelTime = cputime / (ppmMap->width * ppmMap->height);
        CPUTime_Free(&timer);
        
        /* Only writes the time data to a file if the -time flag was used. */
        if (time_file_name != NULL) {
                fp = fopen(time_file_name, "w");
                assert(fp != NULL);
                fprintf(fp, "Total time: %.0f\nTime Per Pixel: %.0f\n", 
                                                                cputime,
                                                                pixelTime);
                fclose(fp);
        }

        Pnm_ppmfree(&ppmMap);

        return EXIT_SUCCESS;

}