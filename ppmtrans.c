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
        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        struct Pnm_rgb *rgb = elem;
        int height = methods->height(array2b);
        *(struct Pnm_rgb *)methods->at(newMap, height - j - 1, i) 
                                                       = *(struct Pnm_rgb *)rgb;
}

void apply180(int i, int j, A2 array2b, void *elem, void *cl)
{
        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        struct Pnm_rgb *rgb = elem;
        int width = methods->width(array2b);
        int height = methods->height(array2b);
        *(struct Pnm_rgb *)methods->at(newMap, width - i - 1, height - j - 1)
                                                       = *(struct Pnm_rgb *)rgb;
}

void apply270(int i, int j, A2 array2b, void *elem, void *cl)
{
        struct mappingCl *bundle = cl;
        A2Methods_T methods = bundle->methods;
        A2 newMap = bundle->newMap;
        int width = methods->width(array2b);
        struct Pnm_rgb *rgb = elem;
        *(struct Pnm_rgb *)methods->at(newMap, j, width - i - 1)
                                                       = *(struct Pnm_rgb *)rgb;
}

void rotate(Pnm_ppm ppmMap, int rotation, A2Methods_mapfun *map, 
                                                            A2Methods_T methods)
{
        if (rotation == 0) {
                Pnm_ppmwrite(stdout, ppmMap);
                return;
        }
        int newHeight;
        int newWidth;
        if(rotation == 90 || rotation == 270) {
                newHeight = methods->width(ppmMap->pixels);
                newWidth = methods->height(ppmMap->pixels);
        } else {
                newWidth = methods->width(ppmMap->pixels);
                newHeight = methods->height(ppmMap->pixels);
        }
        
        A2 newMap = methods->new(newWidth, newHeight, sizeof(struct Pnm_rgb));
        struct mappingCl bundle = {newMap, methods};
        
        if (rotation == 90) {
                map(ppmMap->pixels, apply90, &bundle);
        } else if (rotation == 180) {
                map(ppmMap->pixels, apply180, &bundle);
        } else if (rotation == 270) {
                map(ppmMap->pixels, apply270, &bundle);
        }
        methods->free(&(ppmMap->pixels));
        ppmMap->width = newWidth;
        ppmMap->height = newHeight;
        ppmMap->pixels = newMap; 
        Pnm_ppmwrite(stdout, ppmMap);
}

int main(int argc, char *argv[])
{
        char *time_file_name = NULL;
        char *out_file_name = NULL;
        int   rotation       = 0;
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
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
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
        rotate(ppmMap, rotation, map, methods);
        double cputime = CPUTime_Stop(timer);
        double pixelTime = cputime / (ppmMap->width * ppmMap->height);
        CPUTime_Free(&timer);

        fp = fopen(time_file_name, "w");
        assert(fp != NULL);
        fprintf(fp, "Total time: %.0f\nTime Per Pixel: %.0f\n%s%s\n%s%d", 
                                         cputime, pixelTime, "Transformation: ", 
                                             something, "Rotation: ", rotation);
        fclose(fp);

        Pnm_ppmfree(&ppmMap);

        (void) time_file_name;
}