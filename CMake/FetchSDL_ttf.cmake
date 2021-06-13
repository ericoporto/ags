FetchContent_Declare(
        sdlttf_content
        URL https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-2.0.15.tar.gz
        URL_HASH MD5=33076315f93d9a8e31dfcf314ce6bdbc
)

FetchContent_GetProperties(sdlttf_content)
if(NOT sdlttf_content)
    FetchContent_Populate(sdlttf_content)

    set(SAVED_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")
    set(BUILD_SHARED_LIBS OFF)

    message("Including SDL_ttf ...")

    add_subdirectory(${sdlttf_content_SOURCE_DIR} ${sdlttf_content_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${sdlttf_content_SOURCE_DIR}/)
    set(BUILD_SHARED_LIBS "${SAVED_BUILD_SHARED_LIBS}")

    add_library(SDL2_ttf-interface INTERFACE)
    target_link_libraries(SDL2_ttf-interface INTERFACE SDL2_ttf)
    add_library(SDL2_ttf::SDL2_ttf ALIAS SDL2_ttf-interface)
endif()
