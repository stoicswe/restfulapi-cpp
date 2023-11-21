cmake_minimum_required(VERSION 3.15)

project (
    restfulapicpp
    VERSION 0.1
    DESCRIPTION "An example project for cpp code"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CONAN_CMAKE_CXX_STANDARD 20)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

find_package(CURL REQUIRED)
# Cannot seem to pull down
#find_package(spdlog REQUIRED)

add_subdirectory(src)
add_subdirectory(src/restfulapi)
add_subdirectory(src/logger)