

add_executable(EES2D_App EES2D.cpp)
add_subdirectory(io)
add_subdirectory(utils)
add_subdirectory(mesh)
add_subdirectory(solver)


target_link_libraries(EES2D_App PUBLIC IO Mesh Utils Solver)
