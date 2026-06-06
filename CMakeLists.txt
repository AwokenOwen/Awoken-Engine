cmake_minimum_required(VERSION 3.20)

# Redirect all output to a 'bin' folder in the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(Awoken_Engine)
set(CMAKE_CXX_STANDARD 20)

# Detect MinGW cross-compile
if(CMAKE_CXX_COMPILER MATCHES "mingw" OR CMAKE_CXX_COMPILER MATCHES "w64")
    set(IS_MINGW TRUE)
else()
    set(IS_MINGW FALSE)
endif()

add_library(Awoken_Engine STATIC
        include/Renderer.h
        include/GameManager.h
        include/InputManager.h
        include/LogManager.h
        include/Manager.h
        include/WindowManager.h
        include/WorldManager.h
        include/Event.h
        include/Math.h
        include/Object.h
        include/ResourceManager.h
        include/stb_image.h
        include/ModelRendererComponent.h
        include/Component.h
        include/CameraComponent.h
        Source/Managers/GameManager.cpp
        Source/Managers/InputManager.cpp
        Source/Managers/LogManager.cpp
        Source/Managers/ResourceManager.cpp
        Source/Managers/WindowManager.cpp
        Source/Managers/WorldManager.cpp
        Source/Util/glad.c
        Source/Util/Object.cpp
        Source/Components/Component.cpp
        Source/Components/CameraComponent.cpp
        Source/Components/ModelRendererComponent.cpp
)

target_include_directories(Awoken_Engine PUBLIC
        glad/include
        include
        assimp/include
        freetype/include
        glfw/include
        json/include
        openAL/include
)

target_link_directories(Awoken_Engine PUBLIC
        assimp
        freetype
        glfw
        openAL
)

if(IS_MINGW)
    target_link_libraries(Awoken_Engine PUBLIC
            libassimp.dll.a
            libfreetype.a
            libglfw3.a
            libopenal.dll.a
            opengl32
            gdi32
            winmm
            ole32
            uuid
            imm32
            dwmapi
    )
else()
    target_link_libraries(Awoken_Engine PUBLIC
            libassimp.so
            libfreetype.a
            libglfw3LINUX.a
            libopenal.so
            GL
            dl
            pthread
    )
endif()

file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/bin")
file(COPY gameInit.json DESTINATION "${CMAKE_BINARY_DIR}/bin")

# For testing purposes
project(Awoken_Engine_Test)
add_executable(Awoken_Engine_Test main.cpp)

target_link_libraries(Awoken_Engine_Test PUBLIC Awoken_Engine)

target_include_directories(Awoken_Engine_Test PUBLIC
        glad/include
        include
        assimp/include
        freetype/include
        glfw/include
        json/include
        openAL/include
)