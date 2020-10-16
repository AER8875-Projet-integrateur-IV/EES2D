add_library(Mesh Connectivity.cpp Mesh.cpp Metrics.cpp)

target_include_directories(Mesh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
