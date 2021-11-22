/**
 * @file ppm_example.c
 * @aSaghrouni Khaled Saghrouni Malek
 * @date 12 nvbr 2017
 * @encode text in pictur 
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include "ppm.h"

/**
 * Convert size to binary and stock in the
 * beginning tab of struct txt
 * 21 bits reserve for size of the text 
 * @parm txt
 */
void decimalToBinarySize(txt *Txt) {
        // 21 pour la taille // pour quand puisse devisé l'image 7 pixet (r, g, b) * 3
    int *tab = malloc(21 *sizeof(int));
    static int n = 0;
    int i = 0;
    do {
        tab[i] = Txt->taille1 % 2;
        Txt->taille1 = Txt->taille1 /2;
        i++;
    } while( Txt->taille1 != 0); 

    for (i = 20; i >= 0; i--) {      
        Txt->tab_stock[n] = tab[i];
        n++;
    }    
    free(tab);   
      
}
/**
 * Convert number decimal to binary and stock in the
 * tab of struct txt
 * @parm txt
 */
void decimalToBinary(txt *Txt) {
     
    int *tab = malloc(7* sizeof(int));
    static int n = 21;
    int i = 0;
    do {
        tab[i] = Txt->car % 2;
        Txt->car = Txt->car /2;
        i++;
    } while( Txt->car != 0); 

    for (i = 6; i >= 0; i--) {      
        Txt->tab_stock[n] = tab[i];
        n++;
    }       
     
}

/**
 * read each number of the text and pass it to the functions
 * decimalToBinary 
 * @param param
 * @return NULL
 */

void *dat_Processing(void *param) {
	txt *Txt = (txt *) param;
    FILE *f = fopen(Txt->arg, "r");
  	if (!f) exit(EXIT_FAILURE);
    do {
        Txt->car = fgetc(f);
        if ( feof(f) ) {
           break ;
        }
        decimalToBinary(Txt);
    } while(1); 
    fclose(f);
    return NULL; 
}
/**
 * Write_img it's a function that will go through 
 * all the pixels and verefies the r g and b with the bits of text
 * @param param 
 * @return NULL 
 */
void *write_img(void *param) {
	img_t *img = (img_t *) param;
	int a = 0;
	for (int i = 0; i < img->taille_txt ; i++) {
		pixel_t *p = &img->raw[i];
		if (p->r % 2 != img->tab_stockTxt[a]) {
			if( p->r == 255)
				p->r -= 1;
			else
				p->r += 1;
		}

		if (p->g % 2 != img->tab_stockTxt[a+1]) {
			if( p->g == 255)
				p->g -= 1;
			else
				p->g += 1;
		}	

		if (p->b % 2 != img->tab_stockTxt[a+2]) {
			if( p->b == 255)
				p->b -= 1;
			else
				p->b += 1;
		}		

		a += 3;
		if (a > img->taille_txt) {
			break;
		}
				   
	}
	return NULL;
}

/**
 * Display the program's syntaxe.
 * @param argv program's command line arguments
 */
void usage(char **argv) {
	fprintf(stderr, "usage: %s [-ascii] input output\n"\
			"Where input and output are PPM files and the optional argument\n"\
            "-ascii specifies to write a plain text PPM file.\n", basename(argv[0]));
	exit(EXIT_FAILURE);
}

/**
 * Program entry point.
 * @param argc command line argument count
 * @param argv program's command line arguments
 */
int main(int argc, char **argv) {
    txt *Txt = malloc(sizeof(*Txt));
   	if (!Txt) return EXIT_FAILURE ;

	char *input, *output;
	enum PPM_TYPE type;

	// Parse command line
	if (argc == 4) {
		type = PPM_ASCII;
		Txt->arg = argv[1];
		input = argv[2];
		output = argv[3];
	}
	
	else {
		usage(argv);
	}

	struct  stat st;
    stat(Txt->arg, &st);
    Txt->taille1 = st.st_size;
    Txt->taille = Txt->taille1 * 7 + 21; // 7 bits for the number of bits of a character and 21 for the numbers of bits reserved for the size 
	Txt->tab_stock = malloc(Txt->taille * sizeof(int));

    // creat thread 1
    pthread_t t1;
    if (pthread_create(&t1, NULL, dat_Processing, Txt) != 0) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}	

	if (pthread_join(t1, NULL) != 0) {
   		perror("pthread_join");
		return EXIT_FAILURE;
	}


	// Load image
	img_t *img = load_ppm(input);
    if (!img) {
        fprintf(stderr, "Failed loading \"%s\"!\n", input);
        return EXIT_FAILURE;
    
    }
    
	img->taille_txt = Txt->taille;
	img->tab_stockTxt = malloc(Txt->taille * sizeof(int));

  	img->tab_stockTxt = Txt->tab_stock;
	
	if ( Txt->taille > img->height * img->width){
		fprintf(stderr, "Failed size  %d > %d \n", Txt->taille, (img->height * img->width));
		return EXIT_FAILURE;
	}


	// creat thread 2
	pthread_t t2;
    if (pthread_create(&t2, NULL, write_img, img) != 0) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}	

	if (pthread_join(t2, NULL) != 0) {
   		perror("pthread_join");
		return EXIT_FAILURE;
	}

    //Write image
	if (!write_ppm(output, img, type)) {
		fprintf(stderr, "Failed writing \"%s\"!\n", output);
		free_img(img);
		return EXIT_FAILURE;
	}  

	free(Txt);
	free_img(img);
	return EXIT_SUCCESS;
}
