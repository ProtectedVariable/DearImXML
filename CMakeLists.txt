cmake_minimum_required(VERSION 3.17)
project(DearImXML)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(GLFW_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/")

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


set(EXTRA_SRC "")
if(NOT EXISTS "${GLFW_LIB_PATH}")
	execute_process(COMMAND git clone https://github.com/glfw/glfw.git "${GLFW_LIB_PATH}")
endif()
add_subdirectory(libs/glfw/)

include_directories(includes)
include_directories(DearImXML/src)

set(SRC 
	includes/ImGUI/gl3w.c
	includes/ImGUI/imgui_impl_glfw.cpp
	includes/ImGUI/imgui_impl_opengl3.cpp
	includes/ImGUI/imgui_impl_glfw.h
	includes/ImGUI/imgui_impl_opengl3.h
	includes/ImGUI/imgui.cpp
	includes/ImGUI/imgui_widgets.cpp
	includes/ImGUI/imgui_draw.cpp
	includes/ImGUI/ImCurveEdit.cpp
	includes/ImGUI/ImGradient.cpp
	includes/ImGUI/ImGuizmo.cpp
	includes/ImGUI/ImSequencer.cpp
	includes/ImGUI/imgui_impl_opengl3_loader.h
	includes/ImGUI/imgui_tables.cpp

	DearImXML/src/ImGuiEnum.h
	DearImXML/src/XMLReader.h
	DearImXML/src/XMLTree.h
	DearImXML/src/XMLRenderer.h
	DearImXML/src/XMLEventHandler.h
)
		
add_library(DearImXML SHARED "${SRC}")
add_executable(DearImXMLTest DearImXML/src/main.cpp)
target_link_libraries(DearImXMLTest DearImXML)


file(COPY DearImXML/Assets DESTINATION ${CMAKE_BINARY_DIR})

if(APPLE)
	enable_testing()
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	target_link_libraries(DearImXML glfw ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})
elseif(WIN32)
	target_link_libraries(DearImXML glfw)
else()
	enable_testing()
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})

	add_definitions(${GTK3_CFLAGS_OTHER})

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIRS})

	target_link_libraries(DearImXML glfw ${GTK3_LIBRARIES} ${OPENGL_LIBRARIES} stdc++fs)
endif()