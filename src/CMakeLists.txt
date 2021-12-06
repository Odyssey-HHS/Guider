add_executable(Guider Guider.cpp)

target_link_libraries(Guider PUBLIC Server)
target_link_libraries(Guider PUBLIC Client)