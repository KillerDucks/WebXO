cmake_minimum_required(VERSION 3.10.0)
project(WebXOSrv)

OPTION(DEMO "Install Demo Files." OFF)

# Set the output of the Library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIBS")
# Set the output of the Executable
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/EXE")

# Set Compiler Flags
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Get a list of all CPP Files to use
file(GLOB WebXOB_LIB_SRC
    src/WebXLib/*.cpp
    src/WebXLib/*.hpp
)

# Build the Dynamic Library
add_library(WebXO SHARED ${WebXOB_LIB_SRC})

# Link the Library to needed libs
target_link_libraries(WebXO stdc++fs pthread z)

# Install the WebXO Library
install(
    TARGETS WebXO
    LIBRARY DESTINATION lib
)

# Build the executable
add_executable(${PROJECT_NAME} src/pMain.cpp)

# Link the executable to WebXO 
target_link_libraries(${PROJECT_NAME} WebXO)

# Install WebXO to the bin
install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Install any relevant assets
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/ErrorPages/
    DESTINATION /var/www/ErrorPages
)

# Install the ExampleSite if the demo flag is set
if(DEMO)
    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/ExampleSite/
        DESTINATION /var/www/ExampleSite
    )
endif()


add_subdirectory(Examples)

# Look for GTest
find_package(GTest)
include(CTest)

if (GTEST_FOUND)
    # Enable Testing
    enable_testing()
    # Add the test directory
    add_subdirectory(Tests)
    # Add CTest support
    add_custom_target(run_tests ${CMAKE_CTEST_COMMAND} -V)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
include(CPack)
