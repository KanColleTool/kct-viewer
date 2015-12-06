cmake_minimum_required(VERSION 2.8.12)
project(KCTViewer)

IF(WIN32)
   #Set to allow xp compatibility
  set(CMAKE_GENERATOR_TOOLSET "v120_xp" CACHE STRING "Platform toolset" FORCE)
ENDIF(WIN32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

if(${CMAKE_GENERATOR} MATCHES ".* Makefiles")
	# Add a custom 'make check' target to get output when a test fails
	# Taken from http://stackoverflow.com/questions/16158091/
	if(CMAKE_CONFIGURATION_TYPES)
		add_custom_target(check_viewer COMMAND ${CMAKE_CTEST_COMMAND} 
			--force-new-ctest-process --output-on-failure 
			--build-config "$<CONFIGURATION>")
	else()
		add_custom_target(check_viewer COMMAND ${CMAKE_CTEST_COMMAND} 
			--force-new-ctest-process --output-on-failure)
	endif()
endif()



set_property(TARGET KCTViewer PROPERTY FOLDER "Viewer")
set_property(TARGET KVBase PROPERTY FOLDER "Viewer")
