cmake_minimum_required(VERSION 3.15)
project(py-collapse)

# Compiler settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Include paths
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rd_party")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/eigen
)

# Sources
set(CORE_SOURCES
    src/mesh.cpp
    src/pairs.cpp
)

set(CORE_HEADERS
    src/mesh.hpp
    src/pairs.hpp
    src/types.hpp
)

# Create the core library
add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Add the executable
add_executable(main.o src/main.cpp)  # Note the .o extension
target_link_libraries(main.o PRIVATE core)