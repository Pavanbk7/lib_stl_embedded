cmake_minimum_required(VERSION 3.10)

# Add example executables
add_executable(vector_example vector_example.cpp)
target_link_libraries(vector_example ${PROJECT_NAME})

add_executable(map_example map_example.cpp)
target_link_libraries(map_example ${PROJECT_NAME})
