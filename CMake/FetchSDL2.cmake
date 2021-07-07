FetchContent_Declare(
    sdl2_content
    URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.0.12.tar.gz
    URL_HASH MD5=7ccff5e151cbca26476f6fcaae3ac46c
)

FetchContent_GetProperties(sdl2_content)
if(NOT sdl2_content_POPULATED)
    FetchContent_Populate(sdl2_content)
    if(ANDROID)
        set(SDL_SHARED ON CACHE BOOL "shared")
        set(SDL_STATIC OFF CACHE BOOL "static")
        add_subdirectory(${sdl2_content_SOURCE_DIR} ${sdl2_content_BINARY_DIR} EXCLUDE_FROM_ALL)
        add_library(SDL2::SDL2 ALIAS SDL2)
    elseif(WIN32 OR LINUX OR MACOS OR FREEBSD)
        set(SDL_SHARED OFF CACHE BOOL "shared")
        set(SDL_STATIC ON CACHE BOOL "static")
        set(SDL_STATIC_PIC ON CACHE BOOL "Static version of the library should be built with Position Independent Code")
        set(SDL_SHARED OFF)
        set(SDL_STATIC ON)
        set(SDL_STATIC_PIC ON)
        if(LINUX)
            set(SNDIO_SHARED ON CACHE BOOL "")
            set(SNDIO_SHARED ON)

            # SDL Wayland requires xkbcomon>0.5, see https://github.com/libsdl-org/SDL/issues/4645
            if(PKG_CONFIG_FOUND)
                pkg_check_modules(XKBCOMMON_GT_EQ_0_5  "xkbcommon>=0.5")
                if(NOT XKBCOMMON_GT_EQ_0_5)
                    set(VIDEO_WAYLAND OFF CACHE BOOL "" FORCE)
                    set(VIDEO_WAYLAND_QT_TOUCH OFF CACHE BOOL "" FORCE)
                endif()
            endif()
        endif()
        set(FORCE_STATIC_VCRT ON CACHE BOOL "static windows static vcrc")
        add_subdirectory(${sdl2_content_SOURCE_DIR} ${sdl2_content_BINARY_DIR} EXCLUDE_FROM_ALL)
        add_library(SDL2::SDL2 ALIAS SDL2-static)
    endif()
    add_library(SDL2::SDL2main ALIAS SDL2main)

    file(GLOB INCLUDE_FILES ${sdl2_content_SOURCE_DIR}/include/*.h)
    file(GLOB BIN_INCLUDE_FILES ${sdl2_content_BINARY_DIR}/include/*.h)
    foreach(_FNAME ${BIN_INCLUDE_FILES})
        get_filename_component(_INCNAME ${_FNAME} NAME)
        list(REMOVE_ITEM INCLUDE_FILES ${sdl2_content_SOURCE_DIR}/include/${_INCNAME})
    endforeach()
    list(APPEND INCLUDE_FILES ${BIN_INCLUDE_FILES})

    if(NOT EXISTS "${sdl2_content_BINARY_DIR}/include/SDL2")
        file(MAKE_DIRECTORY "${sdl2_content_BINARY_DIR}/include/SDL2")
        file(COPY ${INCLUDE_FILES}  DESTINATION "${sdl2_content_BINARY_DIR}/include/SDL2")
    endif()
    include_directories("${sdl2_content_BINARY_DIR}/include")

    if(EXISTS "${sdl2_content_SOURCE_DIR}/android-project")
        file(REMOVE_RECURSE "${sdl2_content_SOURCE_DIR}/android-project")
    endif()

    file(COPY CMake/Extra/sdl2-config.cmake DESTINATION ${sdl2_content_BINARY_DIR})
    set(SDL2_DIR ${sdl2_content_BINARY_DIR})
    list(APPEND SDL2_INCLUDE_DIRS "${sdl2_content_BINARY_DIR}/include/SDL2/")
    list(APPEND SDL2_INCLUDE_DIRS "${sdl2_content_BINARY_DIR}/include/")
    list(APPEND SDL2_LIBRARY_DIRS "${sdl2_content_BINARY_DIR}/")
    list(APPEND SDL2_LIBRARIES SDL2::SDL2)
    list(APPEND SDL2_LIBRARIES SDL2::SDL2main)
    set(SDL2MAIN_LIBRARY SDL2::SDL2main)
    set(SDL2_LIBRARY SDL2::SDL2)
endif()

message("SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message("SDL2_LIBRARY: ${SDL2_LIBRARY}")
message("SDL2MAIN_LIBRARY: ${SDL2MAIN_LIBRARY}")
message("SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
