cmake_minimum_required(VERSION 3.16)
project(3D_Renderer)

set(CMAKE_CXX_STANDARD 14)

add_executable(3D_Renderer
        src/main.cpp
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
        src/pipeline/vertex/SimpleVertex.h)