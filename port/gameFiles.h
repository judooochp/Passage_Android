// On Android the game's data files (graphics/*.tga, music/music.tga) are
// packed inside the APK, where the original fopen-based loading code can't
// reach them. prepareGameFiles() copies them to the app's private storage and
// makes that the working directory, so the original code runs unchanged.
//
// On other platforms it does nothing: run the game from the folder that
// holds graphics/ and music/.

#ifndef PORT_GAME_FILES_H
#define PORT_GAME_FILES_H

// Returns false (and prints why) if a file couldn't be copied.
bool prepareGameFiles();

#endif
