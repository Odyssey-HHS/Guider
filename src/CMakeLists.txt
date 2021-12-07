add_executable(Guider Guider.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Guider PRIVATE Threads::Threads)

target_link_libraries(Guider PUBLIC Server)
target_link_libraries(Guider PUBLIC Client)
target_link_libraries(Guider PUBLIC Module)
