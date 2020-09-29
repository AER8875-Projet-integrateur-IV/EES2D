add_executable(EES2D_App Euler2D.cpp)
add_subdirectory(mesh)

target_link_libraries(EES2D_App PUBLIC Mesh)