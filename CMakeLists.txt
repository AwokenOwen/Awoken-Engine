cmake_minimum_required(VERSION 4.1.2)
project(Awoken_Engine)
project(Awoken_Engine_Test)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        Dependencies/Assimp/include
        Dependencies/freetype/include
        Dependencies/glad/include
        Dependencies/glfw/include
        Dependencies/glm/glm
        Dependencies/json/include
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
        TextRenderer.cpp
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