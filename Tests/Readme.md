# WebXO GTest Suite

As the tests use GTest to run you will need to have this installed on your system or the tests will not compile during the CMake build, to install GTest you can refer to the CI/CD pipelines for examples on how to install the library to your system. 

This folder contains all of the tests for WebXO's Dynamic Library. There are assets needed for the tests to run successfully these are stored in the ``./assets/`` folder and should not be changed unless to conduct further testing.

## Running the tests
The tests by default are compiled on a CMake build of the server itself and therefore do not need to be compiled separately.

To run the tests you can use a integrated test running plugin or addon for your IDE, if you do not want to use them you can run the whole suite by changing to the build directory and running ``make run_tests`` this will run the whole suite.

## Generating a XML Report
To generate a XML report of the test results you can run the following command from within the ``./build/EXE`` directory.
```shell
./WebXOSrv_GTests --gtest_output="xml:/<PATH>/WebXOTests.xml"
```
These reports are normally used for the CI/CD pipeline but may offer some benefits for further development.
