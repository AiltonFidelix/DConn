cmake_minimum_required(VERSION 3.16)

project(WSLConn VERSION 0.1 LANGUAGES CXX)

add_compile_definitions(
    VERSION="1.0.0"
    DATE="26/05/2024"
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/wslconn.cpp
    src/wslconn.h
    src/wslinterface.cpp
    src/wslinterface.h
)

set(PROJECT_RESOURCES
    wslconn.qrc
)

set(PROJECT_FORMS
    src/wslconn.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WSLConn
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
        ${PROJECT_FORMS}
    )
endif()

target_link_libraries(WSLConn PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(WSLConn PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WSLConn
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WSLConn)
endif()
