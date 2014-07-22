# 
# Note: This builds the project as an executable linked to a library built from
# its sources. This would be stupid and roundabout if it wasn't for how it also
# lets us share code between application and unit tests with no extra effort.
# 

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

# These three are managed by the version bump script!
set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_PATCH 2)

set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file(version.h.in version.h)



#
# --> Find libraries and stuff
#
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
find_package(Qt5Network REQUIRED)

if(WIN32)
	find_package(Qt5WinExtras)
endif(WIN32)



#
# --> Find sources
#
file(GLOB_RECURSE KCTViewer_FORMS ../forms/*.ui)
file(GLOB_RECURSE KCTViewer_SOURCES *.cpp *.h)
file(GLOB_RECURSE KCTViewer_RESOURCES ../resources/*.qrc)
file(GLOB KCTViewer_MAIN_CPP main.cpp)
list(REMOVE_ITEM KCTViewer_SOURCES ${KCTViewer_MAIN_CPP})

# Remove moc and uic files from the source list
file(GLOB KCTViewer_MOCS moc_*.cpp *_automoc.cpp ui_*.h)
list(REMOVE_ITEM KCTViewer_SOURCES "" ${KCTViewer_MOCS})

qt5_wrap_ui(KCTViewer_UIHEADERS ${KCTViewer_FORMS})
qt5_add_resources(KCTViewer_RESOURCE_SOURCES ${KCTViewer_RESOURCES})




#
# --> Build the common library
#
add_library(KCTVBase ${KCTViewer_SOURCES} ${KCTViewer_UIHEADERS})
target_link_libraries(KCTVBase Qt5::Core Qt5::Widgets Qt5::WebKitWidgets Qt5::Network ${KCTViewer_EXTRALIBS})

if(WIN32)
	target_link_libraries(KCTVBase Qt5::WinExtras wsock32 ws2_32)
endif(WIN32)



#
# --> Build the main application
#
add_executable(KCTViewer WIN32 ${KCTViewer_MAIN_CPP} ${KCTViewer_RESOURCE_SOURCES})
target_link_libraries(KCTViewer KCTVBase)
