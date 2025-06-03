cmake_minimum_required(VERSION 3.0.0)

# Project definition
project(Facsimile LANGUAGES C CXX VERSION 1.0.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add debug warnings
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# Fix for Windows large object files
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif()

# [Optional] Set prefix path for Homebrew (macOS)
if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")  # Adjust if using Intel Mac
endif()

# Dependencies
################################################################################

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND ${PROJECT_NAME}_TEST_LINKED_LIBRARIES PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    Eigen3::Eigen
    Threads::Threads
)

################################################################################

# Subdirectories
add_subdirectory(src)
add_subdirectory(src_test)
add_subdirectory(ExportParaview)

# Collect sources and headers
list(APPEND ${PROJECT_NAME}_sources ${src_sources} ${src_paraview_sources})
list(APPEND ${PROJECT_NAME}_headers ${src_headers} ${src_paraview_headers})
list(APPEND ${PROJECT_NAME}_includes ${src_includes} ${src_paraview_includes})

list(APPEND ${PROJECT_NAME}_TEST_sources ${${PROJECT_NAME}_sources} ${src_test_sources})
list(APPEND ${PROJECT_NAME}_TEST_headers ${${PROJECT_NAME}_headers} ${src_test_headers})
list(APPEND ${PROJECT_NAME}_TEST_includes ${${PROJECT_NAME}_includes} ${src_test_includes})

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Polygons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

################################################################################
# Executables

# Main executable
add_executable(${PROJECT_NAME} main.cpp
    ${${PROJECT_NAME}_sources}
    ${${PROJECT_NAME}_headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_includes})
target_link_libraries(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_LINKED_LIBRARIES})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)

# Test executable
add_executable(${PROJECT_NAME}_TEST main_test.cpp
    ${${PROJECT_NAME}_TEST_sources}
    ${${PROJECT_NAME}_TEST_headers})

target_include_directories(${PROJECT_NAME}_TEST PUBLIC ${${PROJECT_NAME}_TEST_includes})
target_link_libraries(${PROJECT_NAME}_TEST PUBLIC ${${PROJECT_NAME}_TEST_LINKED_LIBRARIES})
target_compile_options(${PROJECT_NAME}_TEST PUBLIC -fPIC)

################################################################################
# Optional: Add CTest integration
enable_testing()
add_test(NAME FacsimileTests COMMAND ${PROJECT_NAME}_TEST)
