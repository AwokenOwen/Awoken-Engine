cmake_minimum_required(VERSION 3.20)

# Redirect all output to a 'bin' folder in the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(Awoken_Engine)
set(CMAKE_CXX_STANDARD 20)

# Detect compiler/toolchain
if(CMAKE_CXX_COMPILER MATCHES "mingw" OR CMAKE_CXX_COMPILER MATCHES "w64")
    set(IS_MINGW TRUE)
    set(IS_MSVC_BUILD FALSE)
elseif(MSVC)
    set(IS_MINGW FALSE)
    set(IS_MSVC_BUILD TRUE)
else()
    set(IS_MINGW FALSE)
    set(IS_MSVC_BUILD FALSE)
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
        include/LinearMath.h
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
        Source/Components/LightComponent.cpp
        include/LightComponent.h
        include/AudioSourceComponent.h
        Source/Components/AudioSourceComponent.cpp
        Source/Components/TextRendererComponent.cpp
        include/TextRendererComponent.h
)

target_include_directories(Awoken_Engine PUBLIC
        include
)

target_link_directories(Awoken_Engine PUBLIC
    lib
)

# MSVC-specific compiler options
if(IS_MSVC_BUILD)
    target_compile_options(Awoken_Engine PRIVATE
            /W3             # Warning level 3
            /MP             # Multi-processor compilation
            /utf-8          # UTF-8 source and execution charset
            /EHsc           # Standard C++ exception handling
    )
    # Suppress common warnings from third-party headers
    target_compile_definitions(Awoken_Engine PUBLIC
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX        # Prevent Windows.h from defining min/max macros
            WIN32_LEAN_AND_MEAN
    )
endif()

if(IS_MINGW)
    target_link_libraries(Awoken_Engine PUBLIC
            libassimp.dll.a
            libfreetype.a
            libglfw3.a
            libopenal.dll.a
            opengl32
    )
elseif(IS_MSVC_BUILD)
    # MSVC expects .lib files for import libs and static libs
    target_link_libraries(Awoken_Engine PUBLIC
            assimp-vc145-mtd.lib
            freetype.lib
            glfw3.lib
            OpenAL32.lib
            opengl32.lib
    )
    # Copy required DLLs to bin alongside the executable
    add_custom_command(TARGET Awoken_Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/assimp/assimp-vc145-mtd.dll"   # Adjust as above
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/openAL/soft_oal.dll"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            COMMENT "Copying DLLs to bin..."
    )
else()
    target_link_libraries(Awoken_Engine PUBLIC
            libassimp.so
            libfreetype.a
            libglfw3LINUX.a
            libopenal.so
            -lz
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

# MSVC: ensure the test executable also gets the right flags
if(IS_MSVC_BUILD)
    target_compile_options(Awoken_Engine_Test PRIVATE /W3 /MP /utf-8 /EHsc)
    target_compile_definitions(Awoken_Engine_Test PUBLIC
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN
    )
endif()