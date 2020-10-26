add_library(Solver Simulation.cpp Schemes.cpp Solver.cpp BoundaryConditions.cpp ConvectiveFlux.h ConservativeVariables.h Residual.h)

target_include_directories(Solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
