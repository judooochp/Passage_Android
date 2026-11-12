#include "touchStick.h"

#include <math.h>

static const float DEAD_ZONE = 0.04f;   // of the shorter screen side
static const float LEASH = 0.13f;

// Finger events arrive through an event watch, which on Android runs on the
// UI thread. Only heldKey is read by the game thread, so only it is atomic.
static SDL_atomic_t heldKey;

static bool tracking = false;
static SDL_FingerID trackedFinger;
static float anchorX, anchorY;   // in units of the shorter screen side


// Convert a finger's normalized (0..1) position to shorter-side units.
static void toStickUnits( const SDL_TouchFingerEvent &inFinger,
                          float *outX, float *outY ) {
    int w = 1, h = 1;
    SDL_Window *window = SDL_GetWindowFromID( inFinger.windowID );
    if( window != NULL ) {
        SDL_GetWindowSize( window, &w, &h );
        }
    float shorter = (float)( w < h ? w : h );
    *outX = inFinger.x * w / shorter;
    *outY = inFinger.y * h / shorter;
    }


static SDL_Keycode directionOf( float inDX, float inDY ) {
    if( sqrtf( inDX * inDX + inDY * inDY ) < DEAD_ZONE ) {
        return 0;
        }
    if( fabsf( inDX ) >= fabsf( inDY ) ) {
        return inDX < 0 ? SDLK_LEFT : SDLK_RIGHT;
        }
    return inDY < 0 ? SDLK_UP : SDLK_DOWN;
    }


static int watchFingers( void *inUnused, SDL_Event *inEvent ) {
    const SDL_TouchFingerEvent &finger = inEvent->tfinger;
    float x, y;

    switch( inEvent->type ) {
        case SDL_FINGERDOWN:
            if( !tracking ) {
                tracking = true;
                trackedFinger = finger.fingerId;
                toStickUnits( finger, &anchorX, &anchorY );
                }
            break;

        case SDL_FINGERMOTION:
            if( tracking && finger.fingerId == trackedFinger ) {
                toStickUnits( finger, &x, &y );
                float dx = x - anchorX;
                float dy = y - anchorY;
                float distance = sqrtf( dx * dx + dy * dy );
                if( distance > LEASH ) {
                    // Pull the anchor along behind the finger.
                    anchorX = x - dx / distance * LEASH;
                    anchorY = y - dy / distance * LEASH;
                    dx = x - anchorX;
                    dy = y - anchorY;
                    }
                SDL_AtomicSet( &heldKey, directionOf( dx, dy ) );
                }
            break;

        case SDL_FINGERUP:
            if( tracking && finger.fingerId == trackedFinger ) {
                tracking = false;
                SDL_AtomicSet( &heldKey, 0 );
                }
            break;
        }
    return 0;
    }


void touchStickInstall() {
    SDL_AtomicSet( &heldKey, 0 );
    SDL_AddEventWatch( watchFingers, NULL );
    }


bool touchStickHeld( SDL_Keycode inKey ) {
    return SDL_AtomicGet( &heldKey ) == (int)inKey;
    }
