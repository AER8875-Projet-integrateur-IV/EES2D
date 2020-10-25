add_library(Solver Simulation.cpp Schemes.cpp Solver.cpp BoundaryConditions.cpp BoundaryConditions.h)

target_include_directories(Solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
