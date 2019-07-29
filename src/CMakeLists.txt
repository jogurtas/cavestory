set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SDL2_PATH ${PROJECT_SOURCE_DIR}/thirdparty/SDL2-2.0.9)
set(SDL2_IMAGE_PATH ${PROJECT_SOURCE_DIR}/thirdparty/SDL2_image-2.0.4)


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

add_executable(cavestory 
				main.cpp 
				Graphics.cpp 
				Game.cpp 
				Input.cpp 
				Sprite.cpp 
				AnimatedSprite.cpp 
				Player.cpp 
				Level.cpp
				Tile.cpp
				AnimatedTile.cpp
				tinyxml2.cpp
				)

target_link_libraries(cavestory ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

# Copy DLLs to output folder on Windows
if (WIN32)
    # Find OS architecture
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(OS_ARCHITECTURE lib/x86)
    else()
        set(OS_ARCHITECTURE lib/x64)
    endif()

    # SDL2
    add_custom_command(TARGET cavestory POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_PATH}/${OS_ARCHITECTURE}/SDL2.dll
            $<TARGET_FILE_DIR:cavestory>)

    # SDL2_image
    file(GLOB DLLS ${SDL2_IMAGE_PATH}/${OS_ARCHITECTURE}/*.dll)
    foreach (DLL ${DLLS})
        add_custom_command(TARGET cavestory POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL} $<TARGET_FILE_DIR:cavestory>)
    endforeach ()
endif ()