cmake_minimum_required(VERSION 3.16)

project(
    app
    VERSION 0.1
    DESCRIPTION "Base C++ CMake application"
    LANGUAGES CXX 
)

if (WIN32)
    set(MSVC_BASE C:/Qt/5.15.2/msvc2019_64)
    #set(MSVC_BASE C:/Qt/6.1.2/msvc2019_64)

    set(G Ninja)
    set(CMAKE_PROJECT_INCLUDE_BEFORE C:/Qt/Tools/QtCreator/share/qtcreator/package-manager/auto-setup.cmake)
    set(QT_QMAKE_EXECUTABLE ${MSVC_BASE}/bin/qmake.exe)
    set(CMAKE_PREFIX_PATH ${MSVC_BASE}/lib/cmake)
    set(CMAKE_MAKE_PROGRAM C:/Qt/Tools/Ninja/ninja.exe)
    #set(Qt6_DIR ${MSVC_BASE}/lib/cmake/Qt6)
    set(Qt5_DIR ${MSVC_BASE}/lib/cmake/Qt5)
elseif (UNIX)
    set(G Ninja)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    set(QT_QMAKE_EXECUTABLE /usr/lib/Qt5/bin/qmake)
    set(CMAKE_PREFIX_PATH /usr/lib/x86_64-linux-gnu/cmake)
    set(Qt5_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5)
    set(Qt5Core_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5Core)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)
#find_package(Qt6 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)
if (WIN32)
    include_directories(${MSVC_BASE}/include)
    include_directories(${MSVC_BASE}/include/QtCore)
    include_directories(${MSVC_BASE}/include/QtGui)
    include_directories(${MSVC_BASE}/include/QtWidgets)
    include_directories(${MSVC_BASE}/include/QtPrintSupport)
elseif (UNIX)
    include_directories(/usr/include/x86_64-linux-gnu/qt5)
    include_directories(/usr/include/x86_64-linux-gnu/qt5/QtCore)
    include_directories(/usr/include/x86_64-linux-gnu/qt5/QtGui)
    include_directories(/usr/include/x86_64-linux-gnu/qt5/QtWidgets)
    include_directories(/usr/include/x86_64-linux-gnu/qt5/QtPrintSupport)
    include_directories(/usr/include/qt5)
    include_directories(/usr/include/qt5/QtCore)
    include_directories(/usr/include/qt5/QtGui)
    include_directories(/usr/include/qt5/QtWidgets)
    include_directories(/usr/include/qt5/QtPrintSupport)
endif ()

add_subdirectory(src)
add_subdirectory(app)
