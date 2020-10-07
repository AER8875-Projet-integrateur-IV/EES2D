add_library(IO Su2Parser.cpp)
target_link_libraries(IO PRIVATE Utils)

target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
