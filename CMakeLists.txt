cmake_minimum_required(VERSION 3.10)

project(Cavestory)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/source/headers ${CMAKE_SOURCE_DIR}/windows-libraries/include ${CMAKE_TOOLCHAIN_FILE}/mingw/include/c++/version)
    link_directories(${CMAKE_SOURCE_DIR}/windows-libraries/lib)
else()
    include_directories(${CMAKE_SOURCE_DIR}/source/headers /usr/include)
    
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()



file(GLOB SRC "source/src/*.cpp" "source/src/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Cavestory ${SRC})

if (WIN32)
    set_target_properties(Cavestory PROPERTIES OUTPUT_NAME "Cavestory")
    target_link_libraries(Cavestory  SDL2main SDL2 SDL2_mixer SDL2_ttf SDL2_image)
else()
    set_target_properties(Cavestory PROPERTIES OUTPUT_NAME "Cavestory.out")
    target_link_libraries(Cavestory 
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)
endif()
