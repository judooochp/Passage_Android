#include "display.h"

#include <stdio.h>

// Phones put camera holes and notches near a short edge, which in landscape
// is the left or right end, where Passage's character walks. Keep this
// fraction of the width clear at each end. 6% clears typical punch-hole
// cameras; on a 2400 px wide screen it is 144 px a side.
#ifdef __ANDROID__
static const float SIDE_MARGIN = 0.06f;
#else
static const float SIDE_MARGIN = 0.0f;
#endif

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int imageWidth = 0;
static int imageHeight = 0;
static const Uint32 *lastPixels = NULL;


bool displayOpen( const char *inTitle, int inImageWidth, int inImageHeight,
                  bool inFullscreen ) {
    imageWidth = inImageWidth;
    imageHeight = inImageHeight;

    // Nearest-neighbour scaling keeps the pixels square and sharp.
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );

    // Windowed size on desktop: 6x, as the original's 640-wide default gave.
    Uint32 flags = SDL_WINDOW_RESIZABLE;
    if( inFullscreen ) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
    window = SDL_CreateWindow( inTitle,
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               inImageWidth * 6, inImageHeight * 6, flags );
    if( window == NULL ) {
        printf( "Couldn't create window: %s\n", SDL_GetError() );
        return false;
        }

    renderer = SDL_CreateRenderer( window, -1, 0 );
    if( renderer == NULL ) {
        printf( "Couldn't create renderer: %s\n", SDL_GetError() );
        return false;
        }

    texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGB888,
                                 SDL_TEXTUREACCESS_STREAMING,
                                 inImageWidth, inImageHeight );
    if( texture == NULL ) {
        printf( "Couldn't create texture: %s\n", SDL_GetError() );
        return false;
        }
    return true;
    }


// Largest whole-number scale that fits inside the side margins, centered.
static SDL_Rect gameRect() {
    int outW, outH;
    SDL_GetRendererOutputSize( renderer, &outW, &outH );

    int usableW = (int)( outW * ( 1 - 2 * SIDE_MARGIN ) );
    int scaleW = usableW / imageWidth;
    int scaleH = outH / imageHeight;
    int scale = scaleW < scaleH ? scaleW : scaleH;
    if( scale < 1 ) {
        scale = 1;
        }

    SDL_Rect r;
    r.w = imageWidth * scale;
    r.h = imageHeight * scale;
    r.x = ( outW - r.w ) / 2;
    r.y = ( outH - r.h ) / 2;
    return r;
    }


void displayShow( const Uint32 *inPixels ) {
    lastPixels = inPixels;
    SDL_UpdateTexture( texture, NULL, inPixels, imageWidth * 4 );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_Rect r = gameRect();
    SDL_RenderCopy( renderer, texture, NULL, &r );
    SDL_RenderPresent( renderer );
    }


void displayRefresh() {
    if( lastPixels != NULL ) {
        displayShow( lastPixels );
        }
    }


void displayToggleFullscreen() {
    Uint32 isFull = SDL_GetWindowFlags( window )
        & SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_SetWindowFullscreen( window,
                             isFull ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP );
    }


void displayClose() {
    if( texture != NULL ) SDL_DestroyTexture( texture );
    if( renderer != NULL ) SDL_DestroyRenderer( renderer );
    if( window != NULL ) SDL_DestroyWindow( window );
    texture = NULL;
    renderer = NULL;
    window = NULL;
    }
