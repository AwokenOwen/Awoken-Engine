cmake_minimum_required(VERSION 4.3)

# Redirect all output to a 'bin' folder in the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(Awoken_Engine)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-Wno-unused-but-set-variable)

include(FetchContent)

FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG master)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW ON)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(FreeType
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG master)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG master)

FetchContent_Declare(OpenAL
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG master)

FetchContent_MakeAvailable(assimp glfw FreeType json OpenAL)

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
        include/MeshRendererComponent.h
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
        Source/Components/MeshRendererComponent.cpp
)

target_include_directories(Awoken_Engine PUBLIC
        glad/include
        include
        ${freetype_SOURCE_DIR}/include
        ${glfw_SOURCE_DIR}/include
        ${json_SOURCE_DIR}/include
        ${openal_SOURCE_DIR}/include
)

target_link_libraries(Awoken_Engine PUBLIC
        assimp::assimp
        freetype
        glfw
        nlohmann_json::nlohmann_json
        OpenAL::OpenAL
)

file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/bin")
file(COPY gameInit.json DESTINATION "${CMAKE_BINARY_DIR}/bin")

