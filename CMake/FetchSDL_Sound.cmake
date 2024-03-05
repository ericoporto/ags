FetchContent_Declare(
    sdlsound_content
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL https://github.com/ericoporto/SDL_sound-1/archive/0187e9a04196c147a4366edd0917811b8d263e16.tar.gz
    URL_HASH SHA1=ac200f41e5dac97729e322a2eaf85b3d1595b0f2
)

FetchContent_GetProperties(sdlsound_content)
if(NOT sdlsound_content)
  FetchContent_Populate(sdlsound_content)
  set(SDLSOUND_BUILD_SHARED off CACHE BOOL "no shared")
  set(SDLSOUND_BUILD_TEST off CACHE BOOL "no tests")
  set(SDLSOUND_BUILD_STATIC on CACHE BOOL "static")

  # see why we need to manually enable here: https://github.com/icculus/SDL_sound/issues/19#issuecomment-1079263491
  set(SDLSOUND_DECODER_MIDI on CACHE BOOL "")

  message("Including SDL2_sound ...")
  message("SDL2_DIR: ${SDL2_DIR}")
  message("sdl2_content_SOURCE_DIR: ${sdl2_content_SOURCE_DIR}")
  add_subdirectory(${sdlsound_content_SOURCE_DIR} ${sdlsound_content_BINARY_DIR} EXCLUDE_FROM_ALL)
  include_directories(${sdlsound_content_SOURCE_DIR}/src/)
  add_library(sdl2_sound-interface INTERFACE)
  target_link_libraries(sdl2_sound-interface INTERFACE SDL2_sound-static)
  add_library(SDL2_sound::SDL2_sound ALIAS sdl2_sound-interface)
endif()
