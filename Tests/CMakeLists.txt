set(BINARY ${CMAKE_PROJECT_NAME}_GTests)

# Get all of the test Files
file(GLOB WebXOB_TEST_SRC
    *.cpp
)

# Build the Test Executable
add_executable(${BINARY} ${WebXOB_TEST_SRC})

# Add the Test Executable
add_test(
    NAME ${BINARY}
    COMMAND ${BINARY}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# Link the executable to WebXO 
target_link_libraries(${BINARY} WebXO gtest)

# gtest_discover_tests(${BINARY})

# Copy over the needed test asset Files
file(
    COPY assets
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)