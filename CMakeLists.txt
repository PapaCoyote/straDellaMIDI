cmake_minimum_required(VERSION 3.15)
project(straDellaMIDI VERSION 1.0.0)

# Find JUCE
find_package(JUCE QUIET)

if(NOT JUCE_FOUND)
    # Try to find JUCE in common locations
    set(JUCE_MODULES_PATH "" CACHE PATH "Path to JUCE modules directory")
    
    if(EXISTS "${JUCE_MODULES_PATH}")
        message(STATUS "Using JUCE modules from: ${JUCE_MODULES_PATH}")
    else()
        message(WARNING "JUCE not found. This CMake file is for convenience testing only.")
        message(WARNING "Please use Projucer to generate proper build files for your platform.")
        message(WARNING "Or set JUCE_MODULES_PATH to your JUCE modules directory.")
    endif()
endif()

# If JUCE is available, create the target
if(JUCE_FOUND OR EXISTS "${JUCE_MODULES_PATH}")
    # This is a placeholder for full build setup
    # The actual build should be done through Projucer-generated files
    add_custom_target(straDellaMIDI_placeholder
        COMMAND echo "Please use Projucer to generate build files for your platform"
        COMMAND echo "This CMakeLists.txt is for reference only"
    )
endif()

# Print help message
message(STATUS "")
message(STATUS "============================================")
message(STATUS "straDellaMIDI - JUCE Application")
message(STATUS "============================================")
message(STATUS "To build this project:")
message(STATUS "1. Open straDellaMIDI.jucer in Projucer")
message(STATUS "2. Select your target platform (Xcode, VS, Linux Makefile, etc.)")
message(STATUS "3. Save the project to generate build files")
message(STATUS "4. Build using the generated IDE project or Makefile")
message(STATUS "============================================")
message(STATUS "")
