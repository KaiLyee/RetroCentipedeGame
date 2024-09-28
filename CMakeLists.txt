# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project information
project(RetroCentipedeGame VERSION 1.0)

# Set the C++ standard to C++17 and ensure it's required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define a common output directory for binaries and libraries
set(COMMON_OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")

# Specify output directories for executables, dynamic, and static libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/bin")  # Executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")  # Dynamic libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")  # Static libraries

# Ensure that executables for all build types are placed in the same output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add source and header files to the project
set(SOURCE_FILES
    codes/Lab1.cpp                # Main game logic
    codes/ECE_Centipede.cpp        # Centipede class implementation
    codes/ECE_Centipede.h          # Centipede class header
    codes/ECE_LaserBlast.cpp       # LaserBlast class implementation
    codes/ECE_LaserBlast.h         # LaserBlast class header
)

# Create the executable file with the specified source files
add_executable(RetroCentipedeGame ${SOURCE_FILES})

# Add the SFML library directory for linking
add_subdirectory(SFML)

# Link the SFML libraries (graphics, window, and system) to the project
target_link_libraries(RetroCentipedeGame sfml-graphics sfml-window sfml-system)

# Copy font folder to output directory
file(COPY ${PROJECT_SOURCE_DIR}/fonts
     DESTINATION "${COMMON_OUTPUT_DIR}/bin")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Include the headers and libraries
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Ensure the headers from the "code" directory are available for inclusion
target_include_directories(RetroCentipedeGame PUBLIC code)

# Copy image folder to output directory
file(COPY ${PROJECT_SOURCE_DIR}/graphic
     DESTINATION "${COMMON_OUTPUT_DIR}/bin")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/graphic 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
