cmake_minimum_required(
        VERSION 3.22
)
project(Games
    VERSION 0.1.0
    LANGUAGES
		CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Don't build examples/tests/install rules for dependencies.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)


if(EMSCRIPTEN)

	set(PLATFORM Web CACHE STRING "" FORCE)

elseif(WIN32)

	set(PLATFORM Win32 CACHE STRING "" FORCE)

elseif(UNIX)

	set(PLATFORM Desktop CACHE STRING "" FORCE)

	# Use X11 on Linux rather than Wayland.
	#set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
	#set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

endif()

add_subdirectory(external/raylib
	${CMAKE_BINARY_DIR}/raylib
	EXCLUDE_FROM_ALL
)

# Build one executable per top-level folder that contains C++ sources.
# Each executable is named after its folder (e.g. Pong, 3DFreeCam).
file(GLOB GAME_DIRS
	LIST_DIRECTORIES true
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*
)

foreach(GAME_DIR IN LISTS GAME_DIRS)
	if(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}
		OR GAME_DIR STREQUAL "external"
		OR GAME_DIR MATCHES "^\\."
		OR GAME_DIR STREQUAL "build")
		continue()
	endif()

	file(GLOB_RECURSE GAME_SOURCES
		CONFIGURE_DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}/*.cc
		${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}/*.cxx
	)

	if(NOT GAME_SOURCES)
		continue()
	endif()

	get_filename_component(GAME_NAME ${GAME_DIR} NAME)

	add_executable(${GAME_NAME} ${GAME_SOURCES})

	target_include_directories(${GAME_NAME}
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}/src
			${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-cpp/include
	)

	target_link_libraries(${GAME_NAME}
		PRIVATE
			raylib
	)
endforeach()
