cmake_minimum_required(VERSION 3.15)
project(py-collapse)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rd_party")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/eigen
)

# library sources
set(CORE_SOURCES
    src/mesh.cpp
    src/pairs.cpp
)

# internal headers
set(CORE_HEADERS
    src/mesh.hpp
    src/pairs.hpp
    src/types.hpp
)

# Create the core library
add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Add the executable
add_executable(main.o src/main.cpp)  # Add this line (adjust path if needed)

# Link the core library to main
target_link_libraries(main PRIVATE core)