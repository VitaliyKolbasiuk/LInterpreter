cmake_minimum_required(VERSION 3.5)

project(Oleksii VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    #set(CMAKE_PREFIX_PATH "~/Qt/6.2.4/macos")
    set(CMAKE_PREFIX_PATH "~/Qt/6.4.0/macos")
    set(BOOST_INCLUDE_DIR "/usr/local/include")
    set(BOOST_LIB_DIR "/usr/local/lib")
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
    set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib")
    
    # for -lcrypto (I do not know who uses it!)
    link_directories(/usr/local/opt/openssl/lib)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        gInterpreter.h
        gInterpreter.cpp
        mainwindow.ui
        ../interpreter/LInterpreter.h
        ../interpreter/Parser.h
        ../interpreter/Log.h
        ../interpreter/Scanner.h
        ../interpreter/SExpr.h
        #../interpreter/interpreter.cpp
        ../interpreter/LInterpreter.cpp
        ../interpreter/pseudoTable.cpp
)
include_directories(../interpreter)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Oleksii
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

add_definitions(-DPROJECT_DIR="${CMAKE_SOURCE_DIR}")

#target_compile_definitions(Oleksii PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(Oleksii PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Oleksii PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Oleksii
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Oleksii)
endif()
