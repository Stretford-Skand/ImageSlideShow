#Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt version: tries Qt6 first, then Qt5. Override with -DQT_DEFAULT_MAJOR_VERSION=5
if(NOT DEFINED QT_DEFAULT_MAJOR_VERSION)
    set(QT_DEFAULT_MAJOR_VERSION 6)
endif()

# Point to a custom Qt installation: cmake -B build -DQT_ROOT_DIR="C:/Qt/6.x/msvc2019_64"
if(DEFINED QT_ROOT_DIR)
    list(PREPEND CMAKE_PREFIX_PATH "${QT_ROOT_DIR}")
endif()

#Options:
option(ADD_RELOAD_BUTTON "Add Reload push button to UI that helps in rescanning and loading image files from folder" ON)
