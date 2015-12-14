# Enable C++11
if(CMAKE_COMPILER_IS_GNUCXX OR APPLE OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

# Don't flood the normal output directories with test binaries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/test)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)

# Allow includes of the normal sources
include_directories("${PROJECT_SOURCE_DIR}/src")
# Include the Catch testing framework
include_directories("${PROJECT_SOURCE_DIR}/vendor/catch/include")

# Find Qt components
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5WebKitWidgets)
find_package(Qt5Network)

if(WIN32)
	find_package(Qt5WinExtras)
endif(WIN32)

# All files called test_*.cpp should be turned into test targets
file(GLOB TEST_SOURCES test_*.cpp)
foreach (path ${TEST_SOURCES})
	get_filename_component(target ${path} NAME_WE)
	add_executable(${target} main.cpp ${path})
	
	target_link_libraries(${target} KVBase Qt5::Core Qt5::Widgets Qt5::WebKitWidgets Qt5::Network)
	if(WIN32)
		target_link_libraries(${target} Qt5::WinExtras wsock32 ws2_32)
	endif(WIN32)
	
	add_test(NAME ${target} COMMAND ${target})
	set_property(TARGET ${target} PROPERTY FOLDER "Tests")
endforeach()
