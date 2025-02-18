cmake_minimum_required(VERSION 3.10)

project(Cavestory)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/include)
else()
    include_directories(${CMAKE_SOURCE_DIR}/source/headers /usr/include)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB SRC "source/src/*.cpp" "source/src/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Cavestory ${SRC})

if (WIN32)
    set_target_properties(Cavestory PROPERTIES OUTPUT_NAME "Cavestory")
else()
    set_target_properties(Cavestory PROPERTIES OUTPUT_NAME "Cavestory.out")
endif()

target_link_libraries(Cavestory 
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)
