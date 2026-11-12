// SDL2 replacement for the SDL 1.2 video surface Passage drew into.
//
// The game builds each frame as a small 0x00RRGGBB pixel image. Here that
// image is uploaded to a texture and drawn scaled up by the largest whole
// number that fits the window, with black borders. This replaces the
// original blowUp code and the 'b' blow-up key.

#ifndef PORT_DISPLAY_H
#define PORT_DISPLAY_H

#include <SDL.h>

// Returns false (and prints why) if the window can't be made.
bool displayOpen( const char *inTitle, int inImageWidth, int inImageHeight,
                  bool inFullscreen );

// inPixels holds inImageWidth * inImageHeight pixels, row by row.
// The pointer is kept so displayRefresh() can draw it again.
void displayShow( const Uint32 *inPixels );

// Draw the last frame again. Needed while the game waits on the title
// screen: a frame presented before the window was fully shown (or before an
// Android app returns from the background) is otherwise lost.
void displayRefresh();

void displayToggleFullscreen();

void displayClose();

#endif
