cmake_minimum_required(VERSION 3.16)

project(DConn VERSION 1.0.0 LANGUAGES CXX)

add_compile_definitions(
    VERSION="${PROJECT_VERSION}"
    DATE="11-20-2026"
    USBIPD_VERSION="4.0.0"
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(HEADERS
    sources/command/command.hpp
    sources/command/commandattach.hpp
    sources/command/commandbind.hpp
    sources/command/commanddetach.hpp
    sources/command/commanddevices.hpp
    sources/command/commandenum.hpp
    sources/command/commandfactory.hpp
    sources/command/commandobserver.hpp
    sources/command/commandservice.hpp
    sources/command/commandversion.hpp
    sources/usbipd/usbipd.hpp
    sources/usbipd/usbipddeviceinfo.hpp
    sources/usbipd/usbipdstateenum.hpp
    sources/usbipd/usbipdstatehelper.hpp
    sources/dconn.hpp
)

set(SOURCES
    sources/command/commandattach.cpp
    sources/command/commandbind.cpp
    sources/command/commanddetach.cpp
    sources/command/commanddevices.cpp
    sources/command/commandservice.cpp
    sources/command/commandversion.cpp
    sources/usbipd/usbipd.cpp
    sources/usbipd/usbipdstatehelper.cpp
    sources/dconn.cpp
    sources/main.cpp
)

set(RESOURCES
    dconn.qrc
    icon.rc
)

set(FORMS
    sources/dconn.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${HEADERS}
        ${SOURCES}
        ${RESOURCES}
        ${FORMS}
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/command
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/usbipd
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
