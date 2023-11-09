cmake_minimum_required(VERSION 3.16)
project(Boids)


## ENABLE LOGGING ##
option(LOGGING "Enable logging" ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp)

# Raylib
set(RAYLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/raylib")
find_library(RAYLIB_LIBRARY NAMES raylib PATHS "${RAYLIB_PATH}/lib" NO_DEFAULT_PATH)
find_path(RAYLIB_INCLUDE_DIR NAMES raylib.h PATHS "${RAYLIB_PATH}/include" NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${RAYLIB_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY})

if(NOT RAYLIB_LIBRARY)
message(FATAL_ERROR "raylib not found : ${RAYLIB_PATH}")
else()
message(STATUS "Found raylib: ${RAYLIB_PATH}")
endif()

# Project sources
# target_sources(${PROJECT_NAME} PRIVATE "src/utils/utils.cpp")

# Project includes
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/utils)


# raygui
set(RAYGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui")
find_library(RAYGUI_LIBRARY NAMES raygui PATHS "${RAYGUI_PATH}" NO_DEFAULT_PATH)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RAYGUI_LIBRARY}" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
target_link_libraries(${PROJECT_NAME} "${RAYGUI_LIBRARY}")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/include)
if(NOT RAYGUI_LIBRARY)
    message(FATAL_ERROR "raygui not found : ${RAYGUI_PATH}")
else()
    message(STATUS "Found raygui: ${RAYGUI_PATH}")
endif()

# spdlog
set(SPDLOG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs")
find_library(SPDLOG NAMES spdlog PATHS "${SPDLOG_PATH}")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodules/logger/include)
target_link_libraries(${PROJECT_NAME} ${SPDLOG})
if(LOGGING)
    add_compile_definitions(LOGGING)
endif()

if(NOT SPDLOG)
    message(FATAL_ERROR "spdlog not found : ${SPDLOG_PATH}")
else()
    message(STATUS "Found spdlog: ${SPDLOG}")
endif()

if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()