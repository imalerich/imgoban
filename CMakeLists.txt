# Cmake required.
cmake_minimum_required (VERSION 2.6)
project (imGoban)

# Version number
set (imGoban_VERSION_MAJOR 0)
set (imGoban_VERSION_MINOR 1)

# Set compiler and linker flags for gcc
set (GCC_COVERAGE_LINK_FLAGS "-lm -fPIC")
set (GCC_COVERAGE_COMPILE_FLAGS "-std=c++14 -Wall")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# Add the binary tree to the search path
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

# Build the binary

add_executable(imGoban
    src/player/roboplayer.cpp
    src/player/userplayer.cpp

    src/imstring.cpp
    src/config.cpp
    src/game.cpp
    src/move.cpp
    src/goban.cpp
    src/state.cpp
    src/main.cpp
)

# Link Allegro with our library

IF (WIN32 AND MINGW)

    target_link_libraries(imGoban
		${CMAKE_SOURCE_DIR}/adelie2d/bin/libAdelie2d.a

		libgdiplus.a
		libuuid.a
		libkernel32.a
		libwinmm.a
		libpsapi.a
		libopengl32.a
		libglu32.a
		libuser32.a
		libcomdlg32.a
		libgdi32.a
		libshell32.a
		libole32.a
		libadvapi32.a
		libws2_32.a
		libshlwapi.a
    )

ELSEIF (WIN32 AND NOT MINGW)

    target_link_libraries(imGoban
		${CMAKE_SOURCE_DIR}/adelie2d/bin/libAdelie2d.lib
    )

ELSEIF (UNIX AND NOT MINGW)
		
    target_link_libraries(imGoban
		imGtp
		Adelie2d
		allegro
		allegro_acodec
		allegro_audio
		allegro_color
		allegro_dialog
		allegro_image
		allegro_memfile
		allegro_physfs
		allegro_primitives
		allegro_font
		allegro_ttf
		vorbis
		dumb
		FLAC
		ogg
		openal
    )

ENDIF()

# Add the install targets
install (TARGETS imGoban DESTINATION bin)

