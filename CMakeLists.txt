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

FetchContent_Declare(
        FreeType
        GIT_REPOSITORY  https://github.com/freetype/freetype.git
        GIT_TAG         master
)

FetchContent_MakeAvailable(FreeType)

find_package(Freetype REQUIRED)

FetchContent_Declare(
        json
        GIT_REPOSITORY  https://github.com/nlohmann/json.git
        GIT_TAG         master
)

FetchContent_MakeAvailable(json)

include_directories(
        Source/glad/include
)

include_directories(
        Source/glad/include
        Source/Managers
        Source/Util
        Source/Components/
        Source/Objects
        ${CMAKE_BINARY_DIR}/_deps/freetype-src/include
        ${CMAKE_BINARY_DIR}/_deps/glfw-src/include
        ${CMAKE_BINARY_DIR}/_deps/json-src/include
        ${CMAKE_BINARY_DIR}/_deps/openal-src/include
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_library(Awoken_Engine STATIC
        Source/Managers/GameManager.cpp
        Source/Managers/GameManager.h
        Source/Managers/LogManager.cpp
        Source/Managers/LogManager.h
        Source/Managers/Manager.h
        Source/Managers/WindowManager.cpp
        Source/Managers/WindowManager.h
        Source/Managers/WorldManager.cpp
        Source/Managers/WorldManager.h
        Source/Objects/Object.cpp
        Source/Objects/Object.h
        Source/Util/Event.h
        Source/Util/glad.c
        Source/Util/Math.h
        Source/Components/Renderer.h
        Source/Components/Component.cpp
        Source/Components/Component.h
)

add_executable(Awoken_Engine_Test
        Source/main.cpp
)

target_link_libraries(Awoken_Engine_Test PRIVATE
        Awoken_Engine
        assimp::assimp
        Freetype::Freetype
        glfw
        OpenAL::OpenAL
)