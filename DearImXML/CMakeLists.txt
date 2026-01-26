cmake_minimum_required(VERSION 3.17)
project(DearImXML)

message(STATUS "Building ${PROJECT_NAME}")

set(IMGUI_SRC)

if(IMXML_INTERNAL_IMGUI)
	set(IMGUI_SRC
		includes/ImGUI/imgui.cpp
		includes/ImGUI/imgui_widgets.cpp
		includes/ImGUI/imgui_draw.cpp
		includes/ImGUI/imgui_tables.cpp
		includes/ImGUI/imgui_stdlib.cpp
		includes/ImGUI/imgui_demo.cpp
	)

endif()

set(SRC
	src/XMLReader.cpp
	src/XMLTree.cpp
	src/XMLRenderer.cpp
 )
		
add_library(DearImXML STATIC
	${SRC}
	${IMGUI_SRC})
target_include_directories(DearImXML PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/includes/ImGUI
	${CMAKE_CURRENT_SOURCE_DIR}/include
	$<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
