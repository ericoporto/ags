set(LINUX_OLD_GCC FALSE)
if(UNIX AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0))
        set(LINUX_OLD_GCC TRUE)
endif()

if(LINUX_OLD_GCC)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/release-1.8.1.tar.gz
            URL_HASH MD5=2e6fbeb6a91310a16efe181886c59596
    )
else()
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/52204f78f94d7512df1f0f3bea1d47437a2c3a58.tar.gz
            URL_HASH MD5=9512a106bb006ab84e0a822ec363c6c7
            DOWNLOAD_EXTRACT_TIMESTAMP true
    )
endif()

message(STATUS "googletest building with LINUX_OLD_GCC: ${LINUX_OLD_GCC}")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24")
    FetchContent_MakeAvailable(googletest)
else()
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)

        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

if(LINUX_OLD_GCC)
    target_compile_options(gtest PRIVATE -Wno-undef -Wno-missing-noreturn -Wno-inline)
    target_compile_options(gtest_main PRIVATE -Wno-undef -Wno-missing-noreturn -Wno-inline)
    # this is because we are building an old googletest before gmock was absorbed by it
    target_compile_options(gmock PRIVATE -Wno-undef -Wno-missing-noreturn -Wno-inline)
    target_compile_options(gmock_main PRIVATE -Wno-undef -Wno-missing-noreturn -Wno-inline)
endif()