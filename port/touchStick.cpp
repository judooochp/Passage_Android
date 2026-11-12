#include "touchStick.h"

#include <math.h>

// Same feel as the web version: there, 14 and 48 CSS px on a phone whose
// short side is ~412 CSS px.
static const float DEAD_ZONE = 0.035f;   // of the shorter screen side
static const float LEASH = 0.12f;

static bool tracking = false;
static SDL_FingerID trackedFinger;
static float anchorX, anchorY;           // in units of the shorter side
static SDL_Keycode heldKey = 0;


// Finger positions arrive normalized 0..1 across the window's width and
// height. Passage only runs wide (Android locks it to landscape), so the
// long side is horizontal; taking the long/short ratio this way stays right
// even if the window size reported mid-rotation is still the portrait one.
static void toStickUnits( const SDL_TouchFingerEvent &inFinger,
                          float *outX, float *outY ) {
    float aspect = 1;
    SDL_Window *window = SDL_GetWindowFromID( inFinger.windowID );
    if( window != NULL ) {
        int w, h;
        SDL_GetWindowSize( window, &w, &h );
        if( w > 0 && h > 0 ) {
            aspect = w > h ? (float)w / h : (float)h / w;
            }
        }
    *outX = inFinger.x * aspect;
    *outY = inFinger.y;
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


static void handleFinger( const SDL_Event &inEvent ) {
    const SDL_TouchFingerEvent &finger = inEvent.tfinger;
    float x, y;

    switch( inEvent.type ) {
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
                heldKey = directionOf( dx, dy );
                }
            break;

        case SDL_FINGERUP:
            if( tracking && finger.fingerId == trackedFinger ) {
                tracking = false;
                heldKey = 0;
                }
            break;
        }
    }


bool touchStickHeld( SDL_Keycode inKey ) {
    // Take any finger events waiting in SDL's queue, in order. Runs on the
    // game thread, so no locking is needed. (While the title screen waits,
    // this isn't called, and the title sees the finger events itself.)
    SDL_Event events[ 32 ];
    int count;
    while( ( count = SDL_PeepEvents( events, 32, SDL_GETEVENT,
                                     SDL_FINGERDOWN, SDL_FINGERMOTION ) )
           > 0 ) {
        for( int i = 0; i < count; i++ ) {
            handleFinger( events[i] );
            }
        }
    return heldKey == inKey;
    }
