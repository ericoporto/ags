FetchContent_Declare(
        sdlttf_content
        URL https://github.com/libsdl-org/SDL_ttf/archive/9363bd0f3b10aad5aaf73a63b9d085aba7ef7098.tar.gz
        URL_HASH MD5=822a4fc8fc6e46a44cdc7668a21c4a06
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
