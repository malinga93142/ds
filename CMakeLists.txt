cmake_minimum_required(VERSION 3.0)
project(MySharedLib)

# Define the source and header files using globbing
file(GLOB SOURCE_FILES src/*.c)
file(GLOB HEADER_FILES include/*.h)

# Set the position-independent code flag for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add the necessary compilation flags
add_compile_options(-Wall -fPIC)

# Create the shared library
add_library(avl SHARED ${SOURCE_FILES})

# Specify the include directory
target_include_directories(avl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify where to install the library and header files
install(TARGETS avl DESTINATION /usr/lib)
install(FILES ${HEADER_FILES} DESTINATION /usr/include)
