cmake_minimum_required(VERSION 3.16)

project(Talhion VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(talhion
    main.cpp
    banco.cpp
    talhoes.cpp
    pragas.cpp
    clima.cpp
    relatorios.cpp
    menu.cpp
    risco.cpp
    alertas.cpp
    historico.cpp
    visualizacao.cpp
)

find_package(SQLite3 REQUIRED)
target_link_libraries(talhion PRIVATE SQLite::SQLite3)

find_path(RAYLIB_INCLUDE_DIR raylib.h)
find_library(RAYLIB_LIBRARY raylib)

if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
    message(FATAL_ERROR "Raylib nao encontrada. Instale a Raylib antes de compilar o Talhion.")
endif()

target_include_directories(talhion PRIVATE ${RAYLIB_INCLUDE_DIR})
target_link_libraries(talhion PRIVATE ${RAYLIB_LIBRARY})

if(WIN32)
    target_link_libraries(talhion PRIVATE opengl32 gdi32 winmm)
endif()
