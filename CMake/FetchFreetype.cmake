FetchContent_Declare(
        freetype_content
        URL https://gitlab.freedesktop.org/freetype/freetype/-/archive/master/freetype-master.tar.gz
        URL_HASH MD5=608129ede6b48570af21df1a481bb4cb
)

FetchContent_GetProperties(freetype_content)
if(NOT freetype_content)
    FetchContent_Populate(freetype_content)

    set(SAVED_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")
    set(SAVED_CMAKE_DISABLE_FIND_PACKAGE_ZLIB_LIBS "${CMAKE_DISABLE_FIND_PACKAGE_ZLIB}")
    set(SAVED_CMAKE_DISABLE_FIND_PACKAGE_BZip2_LIBS "${CMAKE_DISABLE_FIND_PACKAGE_BZip2}")
    set(SAVED_CMAKE_DISABLE_FIND_PACKAGE_PNG_LIBS "${CMAKE_DISABLE_FIND_PACKAGE_PNG}")
    set(SAVED_CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz "${CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz}")
    set(SAVED_CMAKE_DISABLE_FIND_PACKAGE_BrotliDec "${CMAKE_DISABLE_FIND_PACKAGE_BrotliDec}")

    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB ON)
    set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 ON)
    set(CMAKE_DISABLE_FIND_PACKAGE_PNG ON)
    set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz ON)
    set(CMAKE_DISABLE_FIND_PACKAGE_BrotliDec ON)

    message("Including FreeType ...")

    add_subdirectory(${freetype_content_SOURCE_DIR} ${freetype_content_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${freetype_content_SOURCE_DIR}/include/)
    add_library(FreeType::FreeType ALIAS freetype)

    set(FREETYPE_LIBRARY FreeType::FreeType)
    set(FREETYPE_INCLUDE_DIRS ${freetype_content_SOURCE_DIR}/include/)

    set(BUILD_SHARED_LIBS "${SAVED_BUILD_SHARED_LIBS}")
    set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB "${SAVED_CMAKE_DISABLE_FIND_PACKAGE_ZLIB_LIBS}")
    set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 "${SAVED_CMAKE_DISABLE_FIND_PACKAGE_BZip2_LIBS}")
    set(CMAKE_DISABLE_FIND_PACKAGE_PNG "${SAVED_CMAKE_DISABLE_FIND_PACKAGE_PNG_LIBS}")
    set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz "${SAVED_CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz}")
    set(CMAKE_DISABLE_FIND_PACKAGE_BrotliDec "${SAVED_CMAKE_DISABLE_FIND_PACKAGE_BrotliDec}")
endif()
