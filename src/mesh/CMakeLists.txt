add_library(Mesh Mesh.cpp)
target_link_libraries(Mesh PRIVATE Utils)

target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
