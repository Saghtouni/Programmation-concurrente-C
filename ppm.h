/**
 * @file ppm_example.c
 * @aSaghrouni Khaled Saghrouni Malek
 * @date 12 nvbr 2017
 * @encode text in pictur 
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * Store a 24-bit pixel (8-bit per component).
 * @param r the red component
 * @param g the green component
 * @param b the blue component
 */
typedef struct pixel_st {
	uint8_t r, g, b;
} pixel_t;
/**
 * @param taille1 size not include 21 bits
 * @param taille include 21 bits
 * @param tab_stock for stock the bits of the text 
 * @param car the character for text 
 * @param arg 
 */
typedef struct txt {
	int taille1; // pour avoir la taille du caractere dans le texte 
    int taille;
    int *tab_stock;
    int car; 
    char *arg;
    
} txt;
/**
 * Structure holding a 24-bit per pixel image.
 * @param width the width of the image
 * @param height the height of the image
 * @param raw accessor to the image pixel data as a 1D array
 * @param pix accessor to the image pixel data as a 2D array [height][width]
 */
typedef struct img_st {
	int width;
	int height;
	pixel_t *raw;
	pixel_t **pix;
	int taille_txt;
	int *tab_stockTxt;
} img_t;

/**
 * Supported PPM types, either binary or ASCII.
 */
enum PPM_TYPE {
	PPM_BINARY,
	PPM_ASCII
};

extern img_t *alloc_img(int width, int height);
extern void free_img(img_t *img);
extern img_t *load_ppm(char *filename);
extern bool write_ppm(char *filename, img_t *img, enum PPM_TYPE);

