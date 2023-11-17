cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)

project(Sanbox)
set(SRC_LIST src/SandBoxApp.cpp
        src/Sandbox2D.cpp
        src/Sandbox2D.h
        src/GameLayer.cpp
        src/GameLayer.h
        src/Level.cpp
        src/Level.h
        src/Random.cpp
        src/Random.h
        src/Player.cpp
        src/Player.h
        src/Color.h
        src/ParticleSystem.cpp
        src/ParticleSystem.h
)

add_subdirectory(Hazel)

#add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}  hazel)
target_include_directories(${PROJECT_NAME} PRIVATE Hazel/src)
