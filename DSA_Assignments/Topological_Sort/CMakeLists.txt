#The root CMakeLists.txt file.
cmake_minimum_required(VERSION 3.30)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-20" CACHE FILEPATH "The C++ compiler" FORCE)

#The project name is set here. In this case it's Rocket, but you can change it to whatever you want.
project(Rocket  VERSION 1.0.0 LANGUAGES CXX)

# Enable manifest mode and install dependencies automatically
set(VCPKG_MANIFEST_MODE ON)

#Require C++23
set(CXX_STANDARD_REQUIRED ON) #Make C++23 a hard requirement
set(CMAKE_CXX_STANDARD 23) # Default C++ standard for targets
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


#We're using the project name as the target name, but you can change it to make them different.
#With this setup, the name of the executable will be the same as the project name.
# Executable
add_executable(${PROJECT_NAME} 
    main.cpp
)

# Add implementation .cpp files in .include (non-templates)
target_sources(${PROJECT_NAME} PRIVATE
      ./.include/bitvector.cpp
    ./.include/bitmatrix.cpp
        ./.include/linked_list.tpp
    ./.include/dynamic_array.tpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SFML_INCLUDE_DIR} 
    "${CMAKE_SOURCE_DIR}/.include"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Compile warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
