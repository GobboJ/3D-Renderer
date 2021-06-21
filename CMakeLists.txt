cmake_minimum_required(VERSION 3.16)
project(3D_Renderer)

# Find SDL2
set(SDL2_DIR ./include/SDL2-2.0.14)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

# Set Release by default if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")

# If Release, set optimization flag for each system
if(CMAKE_BUILD_TYPE STREQUAL Release)
    if(WIN32)
        message("[!] Windows detected")
        set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    elseif(LINUX) # Linux or else
        message("[!] Linux detected")
        set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    else()
        message("[!] Unknown system")
    endif()
endif()

# CLI Executable
add_executable(3D_Renderer
        src/main/main.cpp
        src/pipeline/Pipeline.cpp
        src/pipeline/Pipeline.h
        src/pipeline/Camera.cpp
        src/pipeline/Camera.h
        src/pipeline/Scene.cpp
        src/pipeline/Scene.h
        src/pipeline/object/Object.cpp
        src/pipeline/object/Object.h
        src/pipeline/object/ObjectInfo.cpp
        src/pipeline/object/ObjectInfo.h
        src/pipeline/util/Vector3.cpp
        src/pipeline/util/Vector3.h
        src/pipeline/DedicatedPipelineImpl.cpp
        src/pipeline/DedicatedPipelineImpl.h
        src/pipeline/DedicatedPipeline.cpp
        src/pipeline/DedicatedPipeline.h
        src/pipeline/vertex/SimpleVertex.cpp
        src/pipeline/vertex/SimpleVertex.h
        src/main/TriangleIterator.cpp
        src/main/TriangleIterator.h
        src/main/SimpleMesh.cpp
        src/main/SimpleMesh.h
        src/main/Shaders.cpp
        src/main/Shaders.h
        src/main/SceneBuilder.cpp
        src/main/SceneBuilder.h src/pipeline/util/bounding_sphere.h)

# SDL Executable
add_executable(3D_Renderer_SDL
        src/main/main_gui.cpp
        src/pipeline/Pipeline.cpp
        src/pipeline/Pipeline.h
        src/pipeline/Camera.cpp
        src/pipeline/Camera.h
        src/pipeline/Scene.cpp
        src/pipeline/Scene.h
        src/pipeline/object/Object.cpp
        src/pipeline/object/Object.h
        src/pipeline/object/ObjectInfo.cpp
        src/pipeline/object/ObjectInfo.h
        src/pipeline/util/Vector3.cpp
        src/pipeline/util/Vector3.h
        src/pipeline/DedicatedPipelineImpl.cpp
        src/pipeline/DedicatedPipelineImpl.h
        src/pipeline/DedicatedPipeline.cpp
        src/pipeline/DedicatedPipeline.h
        src/pipeline/vertex/SimpleVertex.cpp
        src/pipeline/vertex/SimpleVertex.h
        src/main/TriangleIterator.cpp
        src/main/TriangleIterator.h
        src/main/SimpleMesh.cpp
        src/main/SimpleMesh.h
        src/main/Shaders.cpp
        src/main/Shaders.h
        src/main/SceneBuilder.cpp
        src/main/SceneBuilder.h
        src/main/TextureShader.h
        src/pipeline/vertex/TextureVertex.cpp
        src/pipeline/vertex/TextureVertex.h
        src/pipeline/util/ChronoMeter.h src/pipeline/util/ChronoMeter.cpp src/pipeline/util/bounding_sphere.h)

# Link SDL2 lib
target_link_libraries(3D_Renderer_SDL ${SDL2_LIBRARIES})