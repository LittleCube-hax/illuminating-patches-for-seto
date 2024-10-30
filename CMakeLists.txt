cmake_minimum_required(VERSION 3.4)

project(IPSE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

add_executable(IPSE)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/ips.c
)

target_sources(IPSE PRIVATE ${SOURCES})
target_include_directories(IPSE PRIVATE ${CMAKE_SOURCE_DIR}/include)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)

if (WIN32)
    target_link_libraries(IPSE ws2_32)
endif(WIN32)