cmake_minimum_required(VERSION 4.2)
project(Awoken_Engine)
project(Awoken_Engine_Test)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ~/Documents/glfw/include
        ~/Documents/glm/glm
        ~/Documents/glad/include
        ~/Documents/assimp/include
        ~/Documents/freetype/include
        ~/Documents/json/include
        ~/Documents/Awoken-Engine
)

add_library(Awoken_Engine STATIC
        Camera.cpp
        Component.cpp
        DirectionalLight.cpp
        GameManager.cpp
        InputManager.cpp
        Material.cpp
        Mesh.cpp
        MeshRenderer.cpp
        Object.cpp
        PointLight.cpp
        ResourceManager.cpp
        Scene.cpp
        WindowManager.cpp
        WorldManager.cpp
        glad.c
        main.cpp
        TextRenderer.cpp
        TextRenderer.h
)

link_directories(
        ~/Documents/Awoken-Engine/cmake-build-debug
        ~/Documents/glfw/lib
        ~/Documents/glm/lib
        ~/Documents/freetype/lib
)

add_executable(Awoken_Engine_Test
        main.cpp
        glad.c
)

target_link_libraries(Awoken_Engine_Test
        libAwoken_Engine.a
        libglfw3.a
        libglm.a
        libassimp.so
        libfreetype.a
)