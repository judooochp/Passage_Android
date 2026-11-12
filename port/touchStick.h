// Nintendo DS-style "swipe and hold" control.
//
// Put a finger down anywhere: that spot becomes the centre of an invisible
// stick. Drag past a small dead zone and the arrow key in the drag's main
// direction counts as held until the finger lifts or the drag turns. If the
// finger goes far, the centre is pulled along behind it, so reversing
// direction never needs a long drag back.
//
// Lengths are fractions of the screen's shorter side, so the feel is the
// same on any phone.

#ifndef PORT_TOUCH_STICK_H
#define PORT_TOUCH_STICK_H

#include <SDL.h>

// True if the stick is currently holding inKey
// (SDLK_LEFT, SDLK_RIGHT, SDLK_UP or SDLK_DOWN). Call from the game thread
// after SDL_PumpEvents(); it consumes the finger events waiting in SDL's
// queue to update the stick.
bool touchStickHeld( SDL_Keycode inKey );

#endif
