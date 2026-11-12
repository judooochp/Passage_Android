#include "gameFiles.h"

#ifndef __ANDROID__

bool prepareGameFiles() {
    return true;
    }

#else

#include <SDL.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

// Same list as GAME_GRAPHICS in gamma256/gameSource/Makefile.all.
static const char *GAME_FILES[] = {
    "graphics/tileSet.tga",
    "graphics/characterSprite.tga",
    "graphics/characterSpriteSad.tga",
    "graphics/spouseSprite.tga",
    "graphics/numerals.tga",
    "graphics/chest.tga",
    "graphics/chestPrize.tga",
    "graphics/chestDust.tga",
    "graphics/heart.tga",
    "graphics/title.tga",
    "music/music.tga",
    };


// Copy one file out of the APK into the current directory (private storage).
static bool copyAsset( const char *inName ) {
    // On Android, SDL_RWFromFile looks in private storage *before* the APK.
    // From the second launch on, it would find last launch's copy and we'd
    // truncate the file we're reading from. Delete the copy first so the
    // read comes from the APK.
    remove( inName );

    SDL_RWops *in = SDL_RWFromFile( inName, "rb" );
    if( in == NULL ) {
        printf( "Missing asset %s: %s\n", inName, SDL_GetError() );
        return false;
        }
    FILE *out = fopen( inName, "wb" );
    if( out == NULL ) {
        printf( "Can't write %s\n", inName );
        SDL_RWclose( in );
        return false;
        }
    char buffer[ 8192 ];
    size_t count;
    while( ( count = SDL_RWread( in, buffer, 1, sizeof( buffer ) ) ) > 0 ) {
        fwrite( buffer, 1, count, out );
        }
    fclose( out );
    SDL_RWclose( in );
    return true;
    }


bool prepareGameFiles() {
    const char *storage = SDL_AndroidGetInternalStoragePath();
    if( storage == NULL || chdir( storage ) != 0 ) {
        printf( "No internal storage: %s\n", SDL_GetError() );
        return false;
        }
    mkdir( "graphics", 0700 );
    mkdir( "music", 0700 );

    // Copy every launch: it's ~270 KB, and an app update never leaves
    // stale files behind.
    for( const char *name : GAME_FILES ) {
        if( !copyAsset( name ) ) {
            return false;
            }
        }
    return true;
    }

#endif
