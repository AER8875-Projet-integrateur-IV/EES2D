add_library(Mesh Connectivity.cpp Metrics.cpp Mesh.h)

target_include_directories(Mesh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
