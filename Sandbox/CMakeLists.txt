cmake_minimum_required(VERSION 2.8)
project(Sanbox)
set(SRC_LIST src/Application.cpp)

add_subdirectory(engine)


add_executable(${PROJECT_NAME} ${SRC_LIST})
#target_include_directories(${PROJECT_NAME} PUBLIC engine/src)
target_link_libraries(${PROJECT_NAME}  engine)

