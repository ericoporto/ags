cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# We will check_ipo_supported before applying it
if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

cmake_policy(VERSION 3.2)

set(GLM_VERSION "0.9.9")
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)

set(BUILD_STATIC_GLM TRUE)
add_subdirectory(glm)
add_library(glm::glm ALIAS glm)

