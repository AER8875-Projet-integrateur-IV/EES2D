set(TESTNAME test_)                                 # Name for this test executable
set(SOURCE test_Division.cpp                                # List of source files to be included in the executable
            # other cpp file
            # and an other one! 
            )
set(DEPENDANCIES Division                                   # List of all the tests dependancies (every non standard #include )
            # Other library
            # Ouioui vous pouvez en mettre autant que necessaire
            )
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt