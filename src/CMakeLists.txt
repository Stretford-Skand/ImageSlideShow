cmake_minimum_required(VERSION 3.14)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(headers helper.h
			logger.h
			imageextractor.h
			imagestream.h
			centralviewer.h
			issmainwindow.h)

set(sources helper.cpp
			logger.cpp
			imageextractor.cpp
			imagestream.cpp
			centralviewer.cpp
			issmainwindow.cpp
			main.cpp)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

add_executable(image_slideshow ${headers} ${sources} resource.qrc)
target_link_libraries(image_slideshow Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

if(ADD_RELOAD_BUTTON)
	target_compile_definitions(image_slideshow PRIVATE RELOAD_BUTTON)
endif()
