cmake_minimum_required(VERSION 3.10)

project(test_http_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OPENSSL_ROOT_DIR "D:/openSSL/ucrt64")

# 查找openssl库
find_package(OpenSSL REQUIRED)

if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8)
endif()

include_directories(
    ../src/game
    ../src/network
    ../src/util
)

set(SOURCES
    test_http_client.cpp
    ../src/game/gomoku_board.cpp
    ../src/network/network_manager.cpp
    ../src/util/env_util.cpp
)

add_executable(test_http_client ${SOURCES})

# 定义测试环境宏
target_compile_definitions(test_http_client PRIVATE TEST_ENVIRONMENT)

target_link_libraries(test_http_client
    PRIVATE
    ws2_32
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Windows 下通常还需要链接 crypt32 (这是 Windows 的加密 API)
if(WIN32)
    target_link_libraries(test_http_client PRIVATE crypt32)
endif()

target_compile_definitions(test_http_client PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)
