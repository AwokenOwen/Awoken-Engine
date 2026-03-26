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
        Dependencies/CEF
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
        TextRenderer.cpp
)

if(WIN32)

    link_directories(
            cmake-build-debug
            Dependencies/Assimp/lib/windows
            Dependencies/glfw/lib/windows
            Dependencies/freetype/lib/windows
    )

elseif(UNIX)

    link_directories(
            cmake-build-debug
            Dependencies/Assimp/lib/linux
            Dependencies/glfw/lib/linux
            Dependencies/glm/lib/linux
            Dependencies/freetype/lib/linux
            Dependencies/CEF/lib/linux
    )

endif()



add_executable(Awoken_Engine_Test
        main.cpp
        glad.c
)

if(WIN32)

    target_link_libraries(Awoken_Engine_Test PRIVATE
            Awoken_Engine
            assimp-vc143-mt.lib
            freetype.lib
            glfw3.lib
    )

elseif(UNIX)

    target_link_libraries(Awoken_Engine_Test PRIVATE
            Awoken_Engine
            libglfw3.a
            libglm.a
            libassimp.so
            libfreetype.a
            libcef_dll_wrapper.a
            libcef.so
    )

endif()

