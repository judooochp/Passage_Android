# Passage for Android

Jason Rohrer's *Passage* (2007), compiled from the original C++ source into a
native Android app, with a Nintendo DS-style touch control.

Passage and its source are in the public domain:
<http://hcsoftware.sourceforge.net/passage/> (donations welcome there).
This port is unofficial and free.

## Install

On the phone, open
<https://github.com/judooochp/Passage_Android/releases/download/latest/Passage.apk>,
then open the download. Android will ask once to allow installs from your
browser. Each new build installs over the last one.

## Controls

Put a finger anywhere and slide to walk; keep holding to keep walking, lift to
stop. Tap to start from the title screen. Back quits. A gamepad's d-pad also
works.

A credits screen for Jason Rohrer, with a link to his page for the game,
shows at every launch; tap it to reach the title.

## How it's put together

The history is in small steps: commit 1 is the untouched v4 source, commit 2
the SDL2 port, then SDL's Android template unmodified, then the changes to it.

| Path | What it is |
| --- | --- |
| `Passage_v4_UnixSource/` | Rohrer's source. Changed only in `game.cpp` and two `#include` lines in `musicPlayer.cpp` |
| `port/display.*` | SDL2 window and texture, scaled by a whole number (replaces SDL 1.2 video and `blowUp.cpp`) |
| `port/touchStick.*` | The swipe-and-hold control |
| `port/gameFiles.*` | Android only: copies the game's data files out of the APK so the original file code works |
| `CMakeLists.txt` | Builds the game for desktop or Android |
| `android/.../PassageActivity.java` | SDL's activity plus the credits screen |
| `android/` | SDL 2.32.10's `android-project`; SDL itself is a git submodule in `android/app/jni/SDL` |
| `.github/workflows/build-apk.yml` | Builds the APK on each push and publishes it as the `latest` release |

`android/app/debug.keystore` is committed on purpose so every build has the
same signature and can update the installed app. It's a debug key, not fit for
a Play Store release.

## Build on a desktop (for testing)

Needs SDL2 development files and CMake.

    cmake -B build && cmake --build build
    cd Passage_v4_UnixSource/gamma256/gameSource && ../../../build/Passage

`settings/fullscreen.ini` picks fullscreen (1) or a window (0). Arrow keys
move; `f` toggles fullscreen; Esc quits. Set `SDL_MOUSE_TOUCH_EVENTS=1` to try
the touch control with a mouse.

## Build the APK locally

Android Studio, or with the Android SDK + NDK installed:

    git submodule update --init
    cd android && ./gradlew assembleDebug
