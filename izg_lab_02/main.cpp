/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG Lab 02
 * isolony@fit.vutbr.cz
 *
 * $Id: $
 *
 * *) Ovladani programu:
 *      "Leva mys"		- Kresleni cary
 *      "Prava mys"		- Kresleni kruznice
 *      "Prostredni mys"- Rotace aktualniho modelu
 *      "Esc"			- ukonceni programu
 *      "L"				- Nahrani zkusebniho obrazku image.bmp
 *      "S"				- Ulozeni obrazovky do out.bmp
 *      "D"				- Vykresleni usecek ze souboru lines.txt
 *      "T"				- Vykresleni testovaciho vzoru
 *      "B"             - Vykresleni modelu krychle
 *      "N"             - Vykresleni modelu cuboctahedronu
 *      "M"             - Vykresleni modelu ze souboru
 *      "C"				- Vycisteni kreslici plochy
 *
 * Opravy a modifikace:
 * ipolasek@fit.vutbr.cz
 * itomesek@fit.vutbr.cz 
 */

/******************************************************************************
******************************************************************************
 * Includes
 */

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

/* nase veci... */
#include "color.h"
#include "student.h"
#include "io.h"
#include "globals.h"

/* knihovna SDL + system */
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>


/******************************************************************************
 ******************************************************************************
 * Globalni konstanty a promenne
 */

/* titulek hlavniho okna */
const char       * PROGRAM_TITLE      = "IZG 2021 Lab 02";

/* defaultni velikost okna */
const int          DEFAULT_WIDTH      = 800;
const int          DEFAULT_HEIGHT     = 600;
const S_RGBA       DEFAULT_COLOR      = { 205u, 205u, 205u, 255u };

/* kreslici buffer knihovny SDL */
SDL_Surface         * gScreen         = NULL;

/* kreslici buffer IZG cviceni */
S_RGBA				* gFrameBuffer	  = NULL;

/* pomocna promenna pro ukonceni aplikace */
int                 gQuit             = 0;
int					gWidth			  = 800;
int					gHeight			  = 600;

/* Kamera pouzita pro projekce */
Camera              gCamera           = { };

/* Nazvy i/o souboru */
const char        * INPUT_IMAGE_PATH  = "data/image.bmp";
const char        * OUTPUT_IMAGE_PATH = "data/out.bmp";
const char        * LINES_PATH        = "data/lines.txt";
const char        * MODEL_PATH        = "data/model.obj";
const char        * CUBOCTAHEDRON_PATH= "data/cuboctahedron.obj";

/* Data polygonalniho modelu, ktery je aktualne vykreslovan. */
Model               gCurrentModel    = { };

/* Aktualni pozice kurzoru mysi */
int                 gMousePosX       = 0;
int                 gMousePosY       = 0;

/* Body pro vykresleni primitiv */
int                 gMouseBeginX     = 0;
int                 gMouseBeginY     = 0;
int                 gMouseEndX       = 0;
int                 gMouseEndY       = 0;


/******************************************************************************
 ******************************************************************************
 * funkce zajistujici prekresleni obsahu okna programu
 */

void onDraw(void)
{
    /* Test existence frame bufferu a obrazove pameti */
    IZG_ASSERT(gFrameBuffer && gScreen);

    /* Test typu pixelu */
    IZG_ASSERT(gScreen->format->BytesPerPixel == 4);

    /* Kopie bufferu do obrazove pameti */
    SDL_LockSurface(gScreen);

    /* Test, pokud kopirujeme rozdilnou velikost frame_bufferu a rozdilne pameti, musime pamet prealokovat */
    if (gWidth != gScreen->w || gHeight != gScreen->h)
    {
        SDL_SetVideoMode(gWidth, gHeight, 32, SDL_SWSURFACE);
        /*SDL_FreeSurface(gScreen);
        if (!(gScreen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE|SDL_ANYFORMAT)))
        {
            IZG_ERROR("Cannot realocate gScreen buffer");
            SDL_Quit();
        }*/

        gCamera.width = gWidth;
        gCamera.height = gHeight;
    }
    MEMCOPY(gScreen->pixels, gFrameBuffer, sizeof(S_RGBA) * gWidth * gHeight);
    SDL_UnlockSurface(gScreen);
    
    /* Vymena zobrazovaneho a zapisovaneho bufferu */
    SDL_Flip(gScreen);
}


/******************************************************************************
 ******************************************************************************
 * funkce reagujici na stisknuti klavesnice
 * key - udalost klavesnice
 */

void onKeyboard(SDL_KeyboardEvent *key)
{
    /* test existence rendereru */
    IZG_ASSERT(gFrameBuffer);

    /* vetveni podle stisknute klavesy */
    switch( key->keysym.sym )
    {
        /* ukonceni programu - klavesa Esc*/
        case SDLK_ESCAPE:
            gQuit = 1;
            break;

        case SDLK_l:
            /* Stisknuti L tlacitka nahraje obrazek */
            if (loadMyBitmap(INPUT_IMAGE_PATH, &gFrameBuffer, &gWidth, &gHeight))
            { IZG_INFO("File successfully loaded\n"); }
            else
            { IZG_ERROR("Error in loading the file.\n"); }
            break;
        
        case SDLK_s:
            /* Stisknuti S tlacitka ulozi obrazek */
            if (saveMyBitmap(OUTPUT_IMAGE_PATH, &gFrameBuffer, gWidth, gHeight))
            { IZG_INFO("File successfully saved\n"); }
            else
            { IZG_ERROR("Error in saving the file.\n"); }
            break;

        case SDLK_d:
            /* Stisknutim tlacitka D vykreslite usecky ze souboru */
            if (drawLinesFromFile(LINES_PATH)) 
            { IZG_INFO("Drawing lines from the file succeeded\n"); }
            else 
            { IZG_INFO("ERROR in drawing lines from the file\n"); }
            break;

        case SDLK_t:
            /* Stisknutim tlacitka T vykreslite testovaci vzor usecek */
            drawLineTestPattern(gMousePosX, gMousePosY, std::min(gWidth, gHeight) * 0.25f);
            break;

        case SDLK_x:
            drawLineTestPatternAlt();
            break;

        case SDLK_b:
            /* Stisknutim tlacitka B vykreslete krychli */
            gCurrentModel = loadPolygonalCube();
            if (gCurrentModel.faces.empty())
            { IZG_INFO("Failed to load the cube model\n"); }

            clearRaster(DEFAULT_COLOR);
            if (drawPolygonalModel(gCurrentModel, COLOR_BLACK, COLOR_BLACK, gCamera))
            { IZG_INFO("Cube rasterization succeeded\n"); }
            else 
            { IZG_INFO("ERROR in cube rasterization\n"); }
            break;

        case SDLK_n:
            /* Stisknutim tlacitka B vykreslete cuboctahedron */
            gCurrentModel = loadPolygonalModel(CUBOCTAHEDRON_PATH);
            if (gCurrentModel.faces.empty())
            { IZG_INFO("Failed to load the cuboctahedron model\n"); }

            clearRaster(DEFAULT_COLOR);
            if (drawPolygonalModel(gCurrentModel, COLOR_BLACK, COLOR_BLACK, gCamera))
            { IZG_INFO("Cuboctahedron rasterization succeeded\n"); }
            else 
            { IZG_INFO("ERROR in cuboctahedron rasterization\n"); }
            break;

        case SDLK_m:
            /* Stisknutim tlacitka M vykreslete model */
            gCurrentModel = loadPolygonalModel(MODEL_PATH);
            if (gCurrentModel.faces.empty())
            { IZG_INFO("Failed to load the model\n"); }

            clearRaster(DEFAULT_COLOR);
            if (drawPolygonalModel(gCurrentModel, COLOR_BLACK, COLOR_BLACK, gCamera))
            { IZG_INFO("Model rasterization succeeded\n"); }
            else 
            { IZG_INFO("ERROR in model rasterization\n"); }
            break;

        case SDLK_c:
            /* Stisknutim tlacitka C vycistite kreslici plochu */
            clearRaster(DEFAULT_COLOR);
            break;

        default:
            break;
    }
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na pohyb kurzoru mysi
 * mouse - udalost mysi
 */

void onMouseMove(SDL_MouseMotionEvent *mouse)
{
    auto deltaPosX{ mouse->x - gMousePosX };
    auto deltaPosY{ mouse->y - gMousePosY };

    gMousePosX = mouse->x;
    gMousePosY = mouse->y;

    if (mouse->state & SDL_BUTTON_MIDDLE && !gCurrentModel.faces.empty())
    {
        /* Transformace pozice mysi z rasteru na ~<-PI, PI> */
        const auto phi{ (PI - 0.05f) * (gMousePosX * 2.0f / gWidth - 1.0f) };
        /* Transformace pozice mysi z rasteru na ~<-PI, 0.0> */
        const auto theta{ (PI - 0.05f) * (gMousePosY * 0.95f / gHeight - 0.975f) };

        /* Vzdalenost kamery od pocatku */
        const auto distance{ gCamera.position.length() };

        gCamera.position = {
            distance * sinf(phi) * sinf(theta),
            distance * cosf(theta),
            distance * -cosf(phi) * sinf(theta)
        };

        clearRaster(DEFAULT_COLOR);
        drawPolygonalModel(gCurrentModel, COLOR_BLACK, COLOR_BLACK, gCamera);
    }
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na zmacknuti tlacitka mysi
 * mouse - udalost mysi
 */

void onMouseDown(SDL_MouseMotionEvent *mouse)
{
    /* Pokud je zmacknuto tlacitko, ulozime pocatecni bod cary */
    gMouseBeginX = mouse->x;
    gMouseBeginY = mouse->y;
    printf("Info: Beginning / center point %d %d:\n", mouse->x, mouse->y);
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na zmacknuti tlacitka mysi
 * mouse - udalost mysi
 */

void onLeftMouseUp(SDL_MouseMotionEvent *mouse)
{
    /* Pokud je pustene tlacitko, ulozime konecny bod cary a zavolame vykresleni */
    gMouseEndX = mouse->x;
    gMouseEndY = mouse->y;
    printf("Info: End point of line %d %d:\n\n", mouse->x, mouse->y);

    drawLine(gMouseBeginX, gMouseBeginY, gMouseEndX, gMouseEndY);
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na zmacknuti tlacitka mysi
 * mouse - udalost mysi
 */

void onRightMouseUp(SDL_MouseMotionEvent *mouse)
{
    /* Pokud je pustene tlacitko, zistime polomer kruznice a zavolame vykresleni */
    gMouseEndX = mouse->x;
    gMouseEndY = mouse->y;

    const auto radius{ static_cast<float>(sqrt(
        (gMouseEndX - gMouseBeginX) * (gMouseEndX - gMouseBeginX) + 
        (gMouseEndY - gMouseBeginY) * (gMouseEndY - gMouseBeginY)
    )) };
    printf("Info: Radius: %f\n\n", radius);

    drawCircle(gMouseBeginX, gMouseBeginY, radius);
}

/******************************************************************************
 ******************************************************************************
 * funkce tiskne napovedu na stdout
 */
void printHelpText()
{
    IZG_INFO("Application loaded - IZG LAB 02 - Vector Objects Rasterization. Controls:\n"
             "Left mouse click down: \n"
             "    Sets the beginning point of line\n"
             "Left mouse click up: \n"
             "    Sets the ending point of line\n"
             "Right mouse click down: \n"
             "    Sets the center point of circle\n"
             "Right mouse click up: \n"
             "    Sets the radius of circle\n"
             "Middle mouse hold & drag: \n"
             "    Rotate the current polygonal model\n"
             "L: \n"
             "    Load testing image\n"
             "S: \n"
             "    Save current raster into image\n"
             "D: \n"
             "    Draw lines from the lines.txt file\n"
             "T: \n"
             "    Draw line testing pattern\n"
             "B: \n"
             "    Draw polygonal cube\n"
             "N: \n"
             "    Draw polygonal cuboctahedron\n"
             "M: \n"
             "    Draw polygonal model\n"
             "C: \n"
             "    Clear the raster\n"
             "ESC: \n"
             "    Quit application\n"
             "========================================================================\n\n")
}

/******************************************************************************
 ******************************************************************************
 * hlavni funkce programu
 * argc - pocet vstupnich parametru
 * argv - pole vstupnich parametru
 */

int main(int argc, char *argv[])
{
    SDL_Event event;
    
    /* Inicializace SDL knihovny */
    if( SDL_Init(SDL_INIT_VIDEO) == -1 )
    { IZG_SDL_ERROR("Could not initialize SDL library"); }

    /* Nastaveni okna */
    SDL_WM_SetCaption(PROGRAM_TITLE, 0);
   
    /* Alokace frame bufferu (okno + SW zapisovaci */
    if (!(gScreen = SDL_SetVideoMode(gWidth, gHeight, 32, SDL_SWSURFACE)))
    { SDL_Quit(); return 1; }

    if (!(gFrameBuffer = (S_RGBA *)malloc(sizeof(S_RGBA) * gWidth * gHeight)))
    { SDL_Quit(); return 1; }

    printHelpText();
    clearRaster(DEFAULT_COLOR);
    gCurrentModel = loadPolygonalCube();

    /* Kreslime, dokud nenarazime na SDL_QUIT event */
    while(!gQuit) 
    {
        /* Reakce na udalost */
        while( SDL_PollEvent(&event) )
        {
            switch( event.type )
            {
                /* Udalost klavesnice */
                case SDL_KEYDOWN:
                    onKeyboard(&event.key);
                    break;

                /* Udalost mysi */
                case SDL_MOUSEMOTION:
                    onMouseMove(&event.motion);
                    break;

                /* Udalost mysi */
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT || 
                        event.button.button == SDL_BUTTON_RIGHT)
                    { onMouseDown(&event.motion); }
                    break;

                /* Udalost mysi */
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    { onLeftMouseUp(&event.motion); }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    { onRightMouseUp(&event.motion); }
                    break;

                /* SDL_QUIT event */
                case SDL_QUIT:
                    gQuit = 1;
                    break;

                default:
                    break;
            }
        }

        /* Provedeme preklopeni zapisovaciho frame_bufferu na obrazovku*/
        onDraw();
    }

    /* Uvolneni pameti */
    SDL_FreeSurface(gScreen);
    free(gFrameBuffer);
    SDL_Quit();
  
    IZG_INFO("Bye bye....\n\n");
    return 0;
}


/*****************************************************************************/
/*****************************************************************************/
