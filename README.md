# C++ qt
Qt画图板 opencv图像处理
# 简介：
基于 Qt 框架的图形界面程序，涉及图形绘制、交互和处理等功能
# 效果
![image](https://github.com/user-attachments/assets/3bd7f993-cbd7-45a3-abba-30943d5c2be5)
#	使用方法
修改查Cmakelists.txt中依赖的位置
cmake_minimum_required(VERSION 3.28)
project(untitled9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/qt/5.9.2/msvc2017_64")
#包含目录
include_directories("D:/opencv/opencv/build/include")
include_directories("D:/opencv/opencv/build/include/opencv2")
#库目录
link_directories("D:/opencv/opencv/build/x64/vc15/lib")
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        REQUIRED)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(
        ${PROJECT_SOURCE_DIR}/Form
        ${PROJECT_SOURCE_DIR}/Header
)


add_executable(untitled9 main.cpp
        mainwindow.cpp
        Header/mainwindow.h
        Form/mainwindow.ui
        Header/mainwindow.h
        source.qrc
        mygraphicview.cpp
        Header/mygraphicview.h
        StorageAndRead.cpp
        Header/StorageAndRead.h
        sqlite3.c
)
target_link_libraries(untitled9
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
)
#链接静态库
target_link_libraries(untitled9 opencv_world454d.lib)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
