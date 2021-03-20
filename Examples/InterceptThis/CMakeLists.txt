set(BINARY ${CMAKE_PROJECT_NAME}_Example_InterceptThis)

# Set the output of the Executable
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/Examples/Executables")

# Build the executable
add_executable(${BINARY} server.cpp socket2node.cpp)

# Link the executable to WebXO 
target_link_libraries(${BINARY} WebXO)
