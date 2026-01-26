cmake_minimum_required(VERSION 3.17)
project(DearImXMLExample)
set(GLFW_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/")

message(STATUS "Building ${PROJECT_NAME}")

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
)
FetchContent_MakeAvailable(GLFW)

add_executable(DearImXMLExample
	main.cpp
    includes/GL/gl3w.c
    Backend/ImGUI/imgui_impl_opengl3.cpp
	Backend/ImGUI/imgui_impl_glfw.cpp)
target_include_directories(DearImXMLExample PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/includes
	${CMAKE_CURRENT_SOURCE_DIR}/Backend
)
target_link_libraries(DearImXMLExample DearImXML)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
if(APPLE)
	enable_testing()
	find_library(COCOA_LIBRARY Cocoa REQUIRED)
	find_library(OPENGL_LIBRARY OpenGL REQUIRED)
	find_library(IOKIT_LIBRARY IOKit REQUIRED)
	target_link_libraries(DearImXML glfw ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})
elseif(WIN32)
	target_link_libraries(DearImXML glfw)
else()
	enable_testing()
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})

	add_definitions(${GTK3_CFLAGS_OTHER})

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIRS})

	target_link_libraries(DearImXML glfw ${GTK3_LIBRARIES} ${OPENGL_LIBRARIES} stdc++fs)
endif()