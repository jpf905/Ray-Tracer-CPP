cmake_minimum_required(VERSION 3.14)
project(raytracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenMP if available
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "✅ OpenMP found: parallel rendering enabled")
endif()

add_executable(raytracer src/main.cpp)

# Link OpenMP if found
if(OpenMP_CXX_FOUND)
  target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
endif()

# Include Eigen headers (installed via conda)
find_path(EIGEN3_INCLUDE_DIR Eigen/Dense)
if(EIGEN3_INCLUDE_DIR)
  target_include_directories(raytracer PRIVATE ${EIGEN3_INCLUDE_DIR})
endif()
