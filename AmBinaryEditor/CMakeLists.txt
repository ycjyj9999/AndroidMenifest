cmake_minimum_required(VERSION 2.8)
project(AmBinaryEditor)

set(CMAKE_CXX_STANDARD 11)

add_executable(AmBinaryEditor
        AmEditor.cpp
        AmEditor.h
        Log.h
        Main.cpp
        Namespace.cpp
        Namespace.h
        ResChunk.cpp
        ResChunk.h
        ResMap.cpp
        ResMap.h
        StringPoolType.cpp
        StringPoolType.h
        utils.cpp
        utils.h
        XmlTree.cpp
        XmlTree.h)
#add_definitions(-DDEBUG -DINFO -g)
