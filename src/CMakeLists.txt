

add_executable(EES2D_App EES2D.cpp post/postProcess.cpp post/postProcess.h)
add_subdirectory(io)
add_subdirectory(utils)
add_subdirectory(mesh)
add_subdirectory(solver)
add_subdirectory(post)

target_link_libraries(EES2D_App PUBLIC IO Mesh Utils Solver Post)
