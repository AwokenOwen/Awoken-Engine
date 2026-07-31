cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(Awoken_Engine)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(IS_MSVC_BUILD TRUE)
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
        include/LinearMath.h
        include/ft2build.h
        Source/Components/ParticleSystemComponent.cpp
        include/ParticleSystemComponent.h
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
            /W3
            /MP
            /utf-8
            /EHsc
    )
    # Suppress common warnings from third-party headers
    target_compile_definitions(Awoken_Engine PUBLIC
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN
    )
endif()

if(IS_MSVC_BUILD)
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
            "${CMAKE_SOURCE_DIR}/assimp/assimp-vc145-mtd.dll"
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
            libglfw3.a
            libopenal.so
            -lz
    )
endif()

# For testing purposes
file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/bin")
file(COPY gameInit.json DESTINATION "${CMAKE_BINARY_DIR}/bin")

project(Awoken_Engine_Test)
add_executable(Awoken_Engine_Test main.cpp)

target_link_libraries(Awoken_Engine_Test PUBLIC Awoken_Engine)

target_include_directories(Awoken_Engine_Test PUBLIC
        include
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