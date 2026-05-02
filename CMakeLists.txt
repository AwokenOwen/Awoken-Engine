cmake_minimum_required(VERSION 4.3.2)
project(Awoken_Engine)
project(Awoken_Engine_Test)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.3.1)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(OpenAL
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG 1.25.0
        GIT_SHALLOW ON
)
set(ALSOFT_BUILD_ROUTER OFF CACHE BOOL "" FORCE)
set(AL_USE_MODULES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW ON)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")

FetchContent_Declare(FreeType
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG master)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG master)

FetchContent_MakeAvailable(assimp OpenAL glfw FreeType json)

add_library(Awoken_Engine STATIC
        include/Renderer.h
        include/Component.h
        include/GameManager.h
        include/InputManager.h
        include/LogManager.h
        include/Manager.h
        include/WindowManager.h
        include/WorldManager.h
        include/Event.h
        include/Math.h
        include/Object.h
        include/MeshRendererComponent.h
        include/Mesh.h
        include/Material.h
        include/CameraComponent.h
        Source/Components/CameraComponent.cpp
        Source/Components/Component.cpp
        Source/Components/MeshRendererComponent.cpp
        Source/Managers/GameManager.cpp
        Source/Managers/InputManager.cpp
        Source/Managers/LogManager.cpp
        Source/Managers/WindowManager.cpp
        Source/Managers/WorldManager.cpp
        Source/Util/glad.c
        Source/Util/Object.cpp
        Source/Util/Mesh.cpp
        Source/Util/Material.cpp
        main.cpp
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
        OpenAL::OpenAL
        nlohmann_json::nlohmann_json
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

#REMOVE WHEN PUSHING

include_directories(
        glad/include
        include
        ${freetype_SOURCE_DIR}/include
        ${glfw_SOURCE_DIR}/include
        ${json_SOURCE_DIR}/include
        ${openal_SOURCE_DIR}/include
)

add_executable(Awoken_Engine_Test
    main.cpp
)

target_link_libraries(Awoken_Engine_Test PUBLIC
    Awoken_Engine
)