project(asteroids)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp asteroids.cpp
                               skin.cpp)

enable_abcg(${PROJECT_NAME})
