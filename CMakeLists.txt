cmake_minimum_required(VERSION 3.28)
project(Taller)

set(CMAKE_CXX_STANDARD 17)

add_executable(Taller main.cpp
        Grafo.cpp
        Grafo.h
        Nodo.cpp
        Nodo.h
        Sistema.cpp
        Sistema.h)
