

add_executable(EES2D_App EES2D.cpp)
add_subdirectory(io)
add_subdirectory(utils)


target_link_libraries(EES2D_App PUBLIC IO)
