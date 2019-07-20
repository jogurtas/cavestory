cmake_minimum_required(VERSION 3.14)
project(cavestory)

# Find SDL2
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED COMPONENTS main)

# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(cavestory main.cpp)
target_link_libraries(cavestory ${SDL2_LIBS})

# Copy SDL2 DLLs to output folder on Windows
if (WIN32)
    foreach (DLL ${SDL2_DLLS})
        add_custom_command(TARGET cavestory POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:cavestory>)
    endforeach ()
endif ()
