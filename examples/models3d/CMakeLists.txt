project(lookat)
add_executable(${PROJECT_NAME} camera.cpp model.cpp ground.cpp main.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})