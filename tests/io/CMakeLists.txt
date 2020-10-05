set(TESTNAME test_IO)                                 # Name for this test executable
set(SOURCE test_IO.cpp                                # List of source files to be included in the executable
        # other cpp file
        # and an other one!
        )
set(DEPENDANCIES IO                                   # List of all the tests dependancies (every non standard #include )
        )
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt