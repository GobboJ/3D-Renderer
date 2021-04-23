cmake_minimum_required(VERSION 3.17)
project(3D_Renderer)

set(CMAKE_CXX_STANDARD 14)

add_executable(3D_Renderer main.cpp
        pipeline/object/Object.cpp
        pipeline/object/Object.h
        pipeline/vertex/SimpleVertex.cpp
        pipeline/vertex/SimpleVertex.h pipeline/Scene.cpp
        pipeline/Scene.h
        pipeline/object/Object_Impl.h
        pipeline/object/Concrete_Object_Impl.h pipeline/Camera.cpp pipeline/Camera.h pipeline/util/Vector3.cpp pipeline/util/Vector3.h pipeline/Pipeline.cpp pipeline/Pipeline.h)