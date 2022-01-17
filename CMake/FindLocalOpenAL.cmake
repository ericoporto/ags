# Grab system openAL or use embedded mojoAL
# if AGS_PREFER_OPEN_AL_SOFT is true, use fetches OpenAL Soft

if(AGS_PREFER_OPEN_AL_SOFT)
    include(FetchOpenALSoft)
else()

    find_package(OpenAL)

    if(WIN32 OR LINUX OR MACOS OR FREEBSD OR NOT OPENAL_FOUND)
        add_subdirectory(libsrc/mojoAL EXCLUDE_FROM_ALL)

        add_library(openal-interface INTERFACE)
        target_link_libraries(openal-interface INTERFACE MojoAL::MojoAL)
        add_library(External::OpenAL ALIAS openal-interface)
        message("using MojoAL...")
    else()
        add_library(openal-interface INTERFACE)
        target_link_libraries(openal-interface INTERFACE ${OPENAL_LIBRARY})
        target_include_directories(openal-interface INTERFACE ${OPENAL_INCLUDE_DIR})
        add_library(External::OpenAL ALIAS openal-interface)
        message("using system OpenAL...")
    endif()

endif()