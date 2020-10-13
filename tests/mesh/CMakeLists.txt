message("adding test")
# create an exectuable in which the tests will be stored
add_executable(test_Connectivity test_Connectivity.cpp)
# link the Google test infrastructure, mocking library, and a default main fuction to
# the test executable.  Remove g_test_main if writing your own main function.

target_link_libraries(test_Connectivity gtest gmock gtest_main IO Mesh)



# gtest_discover_tests replaces gtest_add_tests,
# see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
gtest_discover_tests(test_Connectivity
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
        )
set_target_properties(test_Connectivity PROPERTIES FOLDER tests)