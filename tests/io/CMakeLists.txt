set(TESTNAME test_Dummy)                                 # Name for this test executable
set(SOURCE test_dummy.cpp                                # List of source files to be included in the executable
        # other cpp file
        # and an other one!
        )
set(DEPENDANCIES Mesh                                   # List of all the tests dependancies (every non standard #include )
        )
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt