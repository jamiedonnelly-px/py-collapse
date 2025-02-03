cmake_minimum_required(VERSION 3.15)
project(py-collapse)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Compiler settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

# Include paths
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rd_party")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/eigen
)

# sources 
file(
    GLOB_RECURSE
    CORE_SOURCES
    src/*.cpp
)

# headers
file(
    GLOB_RECURSE
    CORE_HEADERS
    src/*.hpp
)

# Create the core library
add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(core PUBLIC OpenMP::OpenMP_CXX)

# Add the executable
add_executable(main.o src/main.cpp)  # Note the .o extension
target_link_libraries(main.o PRIVATE core)