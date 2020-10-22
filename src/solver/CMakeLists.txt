add_library(Solver Simulation.cpp Schemes.cpp Solver.cpp)

target_include_directories(Mesh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
