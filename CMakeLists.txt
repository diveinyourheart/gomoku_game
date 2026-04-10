cmake_minimum_required(VERSION 3.10)

project(gomoku_game)

# 设置 Qt 路径
set(CMAKE_PREFIX_PATH "D:/Anaconda/Library")

# 查找 Qt 包
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 处理 UI 文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms)

# 包含目录
include_directories(
    src/game
    src/ui
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

# 源文件
set(SOURCES
    src/main.cpp
    src/game/gomoku_board.cpp
    src/game/gomoku_game.cpp
    src/game/gomoku_player.cpp
    src/ui/mainwindow.cpp
    src/ui/gomoku_widget.cpp
)

# 头文件
set(HEADERS
    src/game/gomoku_board.h
    src/game/gomoku_game.h
    src/game/gomoku_player.h
    src/ui/mainwindow.h
    src/ui/gomoku_widget.h
)

# UI 文件
set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/forms/mainwindow.ui
)

# 资源文件
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc
)

# 创建可执行文件
add_executable(gomoku_game ${SOURCES} ${HEADERS} ${UI_FILES} ${RESOURCES})

# 链接 Qt 库
target_link_libraries(gomoku_game
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)