# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (kestrel)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	external/imgui
	external/imgui_opengl
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB SHADER_FILES ${PROJECT_SOURCE_DIR}/shaders/*.*)
file(GLOB EXTERNAL_FILES 
	${PROJECT_SOURCE_DIR}/external/imgui/*.* 
	${PROJECT_SOURCE_DIR}/external/imgui_opengl/imgui_impl_glfw.h
	${PROJECT_SOURCE_DIR}/external/imgui_opengl/imgui_impl_glfw.cpp)

add_executable(kestrel 
	${SRC_FILES}
	${SHADER_FILES}
	common/shader.cpp
	common/shader.hpp
	shaders/voxel.vertexshader
	shaders/voxel.fragmentshader
	${EXTERNAL_FILES}
	)

target_link_libraries(kestrel
	${ALL_LIBS}
)

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

SOURCE_GROUP(external\\imgui REGULAR_EXPRESSION ".*/external/imgui/.*" )
SOURCE_GROUP(external\\imgui_opengl REGULAR_EXPRESSION ".*/external/imgui_opengl/.*" )