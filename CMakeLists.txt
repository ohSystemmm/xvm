cmake_minimum_required(VERSION 3.16)

project(xvm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)

add_executable(xvm
    src/main.cpp
)

target_link_libraries(xvm PRIVATE SDL3::SDL3)

if(WIN32)
    add_custom_command(TARGET xvm POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:xvm>
        COMMENT "Copying SDL3 runtime DLL to output directory"
    )
endif()