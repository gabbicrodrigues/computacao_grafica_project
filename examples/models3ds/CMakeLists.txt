project(models3ds)
add_executable(${PROJECT_NAME} camera.cpp model.cpp  main.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})