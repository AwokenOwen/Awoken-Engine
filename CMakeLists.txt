cmake_minimum_required(VERSION 4.1.2)
project(Awoken_Engine)
project(Awoken_Engine_Test)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Declare and make Assimp available
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.3.1 # Recommended to use a specific version tag
)

# Optional: Disable unnecessary Assimp features for faster builds
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

# Declare the OpenAL Soft dependency from its GitHub repository
FetchContent_Declare(
        OpenAL
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG        master # You can specify a specific release tag or commit hash here
)

# Make the content available and add its CMake targets to the project
FetchContent_MakeAvailable(OpenAL)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        "3.4" # Use the desired version tag, e.g., "3.4"
        GIT_SHALLOW    ON    # Use shallow clone to save time and space
)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL BOOL "") # Disable building documentation
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL BOOL "") # Disable building tests
set(GLFW_INSTALL OFF CACHE INTERNAL BOOL "")   # Disable the install target

FetchContent_MakeAvailable(glfw)


find_package(Freetype REQUIRED)

include_directories(
        Dependencies/freetype/include
        Dependencies/glad/include
        cmake-build-debug/_deps/glfw-src/include
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
        TextRenderer.cpp
        AudioManager.cpp
)

add_executable(Awoken_Engine_Test
        main.cpp
        glad.c
)


target_link_libraries(Awoken_Engine_Test PRIVATE
        Awoken_Engine
        assimp::assimp
        Freetype::Freetype
        glfw
        OpenAL::OpenAL
)