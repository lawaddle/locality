


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

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] "
		        "[-time time_file] "
		        "[filename]\n",
                        progname);
        exit(1);
}

void apply90(int i, int j, A2 array2b, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2b != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2b));
        assert(j >= 0 && j < methods->height(array2b));


        struct Pnm_rgb *rgb = elem;
        int height = methods->height(array2b);
        *(struct Pnm_rgb *)methods->at(newMap, height - j - 1, i) 
                                                       = *(struct Pnm_rgb *)rgb;
}

void apply180(int i, int j, A2 array2b, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2b != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2b));
        assert(j >= 0 && j < methods->height(array2b));

        struct Pnm_rgb *rgb = elem;
        int width = methods->width(array2b);
        int height = methods->height(array2b);
        *(struct Pnm_rgb *)methods->at(newMap, width - i - 1, height - j - 1)
                                                       = *(struct Pnm_rgb *)rgb;
}

void apply270(int i, int j, A2 array2b, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2b != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2b));
        assert(j >= 0 && j < methods->height(array2b));
        
        int width = methods->width(array2b);
        struct Pnm_rgb *rgb = elem;
        *(struct Pnm_rgb *)methods->at(newMap, j, width - i - 1)
                                                       = *(struct Pnm_rgb *)rgb;
}

void applyTranspose(int i, int j, A2 array2b, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2b != NULL);
        assert(elem != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2b));
        assert(j >= 0 && j < methods->height(array2b));
        
        struct Pnm_rgb *rgb = elem;
        *(struct Pnm_rgb *)methods->at(newMap, j, i) = *(struct Pnm_rgb *)rgb;
}

void applyHorizontal(int i, int j, A2 array2b, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2b != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2b));
        assert(j >= 0 && j < methods->height(array2b));


        struct Pnm_rgb *rgb = elem;
        int height = methods->height(array2b);
        *(struct Pnm_rgb *)methods->at(newMap, i, height - j - 1)
                                                       = *(struct Pnm_rgb *)rgb;
}

void applyVertical(int i, int j, A2 array2b, void *elem, void *cl)
{
        assert(cl != NULL);
        assert(array2b != NULL);

        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        assert(methods != NULL);
        assert(newMap != NULL);
        assert(i >= 0 && i < methods->width(array2b));
        assert(j >= 0 && j < methods->height(array2b));


        struct Pnm_rgb *rgb = elem;
        int width = methods->width(array2b);
        *(struct Pnm_rgb *)methods->at(newMap, width - i - 1, j)
                                                       = *(struct Pnm_rgb *)rgb;
}

Pnm_ppm transform(Pnm_ppm ppmMap, int transformation, A2Methods_mapfun *map, 
                                                            A2Methods_T methods)
{
        assert(methods != NULL);
        assert(map != NULL);
        assert(*map != NULL);
        assert(ppmMap != NULL);
        
        if (transformation == ZERO) {
                return ppmMap;
        }
        int newHeight = methods->height(ppmMap->pixels);
        int newWidth = methods->width(ppmMap->pixels);
        if (transformation == NINETY || 
            transformation == TWO_SEVENTY || 
            transformation == TRANSPOSE) {
                newHeight = methods->width(ppmMap->pixels);
                newWidth = methods->height(ppmMap->pixels);
        }
        
        A2 newMap = methods->new(newWidth, newHeight, sizeof(struct Pnm_rgb));
        struct mappingCl bundle = {newMap, methods};
        
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

        CPUTime_T timer = CPUTime_New();
        CPUTime_Start(timer);
        Pnm_ppm rotated = transform(ppmMap, transformation, map, methods);
        double cputime = CPUTime_Stop(timer);
        Pnm_ppmwrite(stdout, rotated);
        double pixelTime = cputime / (ppmMap->width * ppmMap->height);
        CPUTime_Free(&timer);
        
        if (time_file_name != NULL) {
                fp = fopen(time_file_name, "w");
                assert(fp != NULL);
                fprintf(fp, "Total time: %.0f\nTime Per Pixel: %.0f\n", cputime, 
                                                                     pixelTime);
                fclose(fp);
        }

        Pnm_ppmfree(&ppmMap);

        return EXIT_SUCCESS;

}