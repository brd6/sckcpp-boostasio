cmake_minimum_required(VERSION 3.8)

# Package manager
include("cmake/Modules/HunterGate.cmake")
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.19.216.tar.gz"
        SHA1 "247d3ca881e9c83e6ac419d55cd08d1775ea077a"
)

project(sckcpp)

set(CMAKE_CXX_STANDARD 11)

# cmake modules path
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

hunter_add_package(Boost COMPONENTS program_options system filesystem)
find_package(Boost CONFIG REQUIRED program_options system filesystem)


# Include directory
set(SCKCPP_INCLUDE ${CMAKE_SOURCE_DIR}/include)
include_directories(${SCKCPP_INCLUDE})

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

# Source files
set(SOURCE_FILES ${SOURCE_FILES} ${INCLUDE_FILES})

# Create library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Link library
target_link_libraries(${PROJECT_NAME}
        Boost::system
        Boost::filesystem
        Boost::program_options
        )


# Install
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)
INSTALL(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
