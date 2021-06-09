/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;


/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
    if (((x < 0) || (x >= width)) || ((y < 0) || (y >= height))){
        return COLOR_BLACK;
    }
    
    S_RGBA color = frame_buffer[x+y*width];

    return color;
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
    if (((x < 0) || (x >= width)) || ((y < 0) || (y >= height))){
        return;
    }

    frame_buffer[x+y*width] = color;

}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
    int intensity;
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            S_RGBA color = getPixel(x,y);
            intensity = ROUND(0.299*color.red + 0.587*color.green + 0.114*color.blue);
            color.red = color.green = color.blue = intensity;
            putPixel(x, y, color);
        }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
    grayScale();

    int i,j;
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            i = x % M_SIDE;
            j = y % M_SIDE;
            S_RGBA color = getPixel(x,y);
            
            if (color.red > M[i+j*M_SIDE]){
                putPixel(x,y, COLOR_WHITE);
            } else {
                putPixel(x,y, COLOR_BLACK);
            }
        }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */

void change_neighbor(int x, int y, int error){
    S_RGBA color = getPixel(x,y);
    double new_color = color.red + error;

    if (new_color > 255) {
        new_color = 255;
    } else if (new_color < 0) {
       new_color = 0;
    }
    color.red = color.blue = color.green = new_color;
    putPixel(x, y, color);
}

void errorDistribution()
{   
    grayScale();
  
    const int threshold = 128;
    int error;

    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
	    S_RGBA color = getPixel(x, y);
           
            if (color.red > threshold) {
                error = color.red - 255;
                putPixel(x, y, COLOR_WHITE);
            } else {
                error = color.red;
                putPixel(x, y, COLOR_BLACK);
            }
            
            change_neighbor(x+1, y, ROUND(3*error/8));
            change_neighbor(x, y+1, ROUND(3*error/8));
            change_neighbor(x+1, y+1, ROUND(2*error/8));
        }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/
