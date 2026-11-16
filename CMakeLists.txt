cmake_minimum_required(VERSION 3.16)

project(xvm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(xvm
    src/main.cpp
    src/render/renderer.cpp
    src/log/logger.cpp
)

target_include_directories(xvm PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(xvm PRIVATE SDL3::SDL3 SDL3_image::SDL3_image spdlog::spdlog)

# Logging mode: pass -DLOG_TO_FILE from the Makefile / CLI to enable file
# logging in addition to stdout.  When the option is OFF (the default),
# only stdout logging is active.
option(XVM_LOG_TO_FILE "Enable file-based logging alongside stdout" OFF)
if(XVM_LOG_TO_FILE)
    target_compile_definitions(xvm PRIVATE LOG_TO_FILE)
endif()

if(WIN32)
    add_custom_command(TARGET xvm POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:xvm>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:xvm>
        COMMENT "Copying SDL3 and SDL3_image runtime DLLs to output directory"
    )
endif()
