cmake_minimum_required(VERSION 3.17)
project(DearImXMLRoot)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD 20)

option(BUILD_IMXML_EXAMPLE "Build ImXML example app" ON)
option(IMXML_INTERNAL_IMGUI "Build ImXML with it's internal ImGui version'" ON)

add_subdirectory(DearImXML)
if(BUILD_IMXML_EXAMPLE)
	add_subdirectory(example)
endif()