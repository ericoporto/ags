FetchContent_Declare(
        openalsoft_content
        URL https://github.com/kcat/openal-soft/archive/refs/tags/1.21.1.tar.gz
        URL_HASH MD5=a4922a79526c590b6cac0c10f3f1bef8
)

FetchContent_GetProperties(openalsoft_content)
if(NOT openalsoft_content)
    FetchContent_Populate(openalsoft_content)

    file(COPY_FILE "CMake/Extra/OpenALSoft_CMakeLists.txt" "${openalsoft_content_SOURCE_DIR}/CMakeLists.txt")

    set(LIBTYPE "STATIC" CACHE STRING "STATIC")
    add_subdirectory(${openalsoft_content_SOURCE_DIR} ${openalsoft_content_BINARY_DIR} EXCLUDE_FROM_ALL)


    add_library(openal-interface INTERFACE)
    target_link_libraries(openal-interface INTERFACE OpenAL)
    target_include_directories(openal-interface INTERFACE ${openalsoft_content_SOURCE_DIR}/include/)
    add_library(External::OpenAL ALIAS openal-interface)
    message("using fetched OpenAL Soft...")
endif()