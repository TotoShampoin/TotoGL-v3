cmake_minimum_required(VERSION 3.5)
project(totogl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/" ${CMAKE_MODULE_PATH})

set(TOTOGL_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(TOTOGL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(TOTOGL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TOTOGL_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE TOTOGL_INCLUDE_FILES CONFIGURE_DEPENDS ${TOTOGL_INCLUDE_DIR}/*.hpp ${TOTOGL_INCLUDE_DIR}/*.tpp)
file(GLOB_RECURSE TOTOGL_SOURCE_FILES CONFIGURE_DEPENDS ${TOTOGL_SOURCE_DIR}/*.cpp)

include(CMakeUtils/files_and_folders.cmake)
include(${TOTOGL_VENDOR_DIR}/imgui.cmake)

add_library(${PROJECT_NAME} SHARED ${TOTOGL_INCLUDE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TOTOGL_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL glfw GLU GLEW glm)

target_include_directories(${PROJECT_NAME} PUBLIC ${TOTOGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${TOTOGL_VENDOR_DIR}/includes)


add_executable(${PROJECT_NAME}_test ${TOTOGL_SOURCE_FILES})
target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_23)

set_target_properties(${PROJECT_NAME}_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TOTOGL_BINARY_DIR})
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} OpenGL glfw GLU GLEW glm)

Cool__target_copy_folder(${PROJECT_NAME}_test assets)

if (MSVC)
    target_compile_options(${PROJECT_NAME}_test PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()
