add_library(IO Su2Parser.cpp VtuWriter.h VtuWriter.cpp InputParser.cpp InputParser.h)

target_include_directories(IO PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
