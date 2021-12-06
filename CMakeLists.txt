cmake_minimum_required(VERSION 3.10)

# set the project name
project(Guider)

add_subdirectory(inc)
add_subdirectory(src)

target_include_directories(Guider PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/inc"
                          "${PROJECT_SOURCE_DIR}/src"
                          )