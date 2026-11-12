# Builds Passage against SDL2.
#   Desktop (for testing):  cmake -B build && cmake --build build
#                           then run build/Passage from gamma256/gameSource
#   Android:                included by android/app/jni/CMakeLists.txt,
#                           which builds SDL2 first; output is libmain.so
cmake_minimum_required(VERSION 3.10)
project(Passage CXX)

set(CMAKE_CXX_STANDARD 11)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/Passage_v4_UnixSource)
set(GAME ${SRC}/gamma256/gameSource)
set(MG ${SRC}/minorGems)

# Same lists as Makefile.all, minus blowUp.cpp (scaling moved to port/display).
set(PASSAGE_SOURCES
    ${GAME}/game.cpp
    ${GAME}/landscape.cpp
    ${GAME}/World.cpp
    ${GAME}/map.cpp
    ${GAME}/common.cpp
    ${GAME}/score.cpp
    ${GAME}/musicPlayer.cpp
    ${GAME}/Timbre.cpp
    ${GAME}/Envelope.cpp
    ${MG}/io/file/linux/PathLinux.cpp
    ${MG}/util/stringUtils.cpp
    ${MG}/util/StringBufferOutputStream.cpp
    ${MG}/util/SettingsManager.cpp
    ${MG}/system/unix/TimeUnix.cpp
    ${MG}/system/linux/ThreadLinux.cpp
    ${MG}/crypto/hashes/sha1.cpp
    ${MG}/formats/encodingUtils.cpp
    port/display.cpp
    port/touchStick.cpp
    port/gameFiles.cpp
)

if(ANDROID)
    # SDLActivity loads the game from libmain.so.
    add_library(main SHARED ${PASSAGE_SOURCES})
    set(PASSAGE_TARGET main)
    target_link_libraries(main SDL2)
else()
    find_package(SDL2 REQUIRED)
    add_executable(Passage ${PASSAGE_SOURCES})
    set(PASSAGE_TARGET Passage)
    target_link_libraries(Passage SDL2::SDL2 pthread)
endif()

target_include_directories(${PASSAGE_TARGET} PRIVATE ${SRC} ${CMAKE_CURRENT_SOURCE_DIR})
# The original code is old C++; keep its warnings out of the way.
target_compile_options(${PASSAGE_TARGET} PRIVATE -w)
