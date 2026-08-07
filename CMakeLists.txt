cmake_minimum_required(VERSION 3.20)

project(SFML_Game
    VERSION 1.0.0
    LANGUAGES CXX
)

# ---- C++ 标准 ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- 输出目录（exe 和 dll 都在 build/bin 下，方便运行）----
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ---- 定位预编译 SFML 库 ----
# 假设你的预编译包解压在 third_party/SFML-3.1.0 下，
# 其中 cmake 配置文件位于 third_party/SFML-3.1.0/lib/cmake/SFML
# 这里显式设置 SFML_DIR，避免 find_package 去系统路径乱找
set(SFML_DIR "${CMAKE_SOURCE_DIR}/third_party/SFML-3.1.0/lib/cmake/SFML")
find_package(SFML 3.1 REQUIRED COMPONENTS Graphics Window System Audio Network)

# ---- 收集你的源代码 ----
set(SOURCES
    "${CMAKE_SOURCE_DIR}/include/game.h"
    "${CMAKE_SOURCE_DIR}/src/game.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
 "include/resource_holder.h")

# ---- 创建可执行文件 ----
add_executable(${PROJECT_NAME} ${SOURCES})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# ---- 包含你自己的头文件目录 ----
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

# ---- 链接 SFML 库 ----
target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
)

# ---- 自动复制 SFML DLL 到 exe 旁边（Windows 动态库） ----
if(WIN32 AND SFML_BIN_DIR)
    # SFML 的 cmake 配置会定义 SFML_BIN_DIR 变量，指向预编译包中的 bin 目录
    # 使用 generator expression 复制所有需要的 DLL
    file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
    foreach(dll ${SFML_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        )
    endforeach()
elseif(WIN32)
    # 如果 SFML_BIN_DIR 未定义，则手动指定路径（根据你的预编译包结构调整）
    set(SFML_BIN_PATH "${CMAKE_SOURCE_DIR}/third_party/SFML-3.1.0/bin")
    if(EXISTS "${SFML_BIN_PATH}")
        file(GLOB SFML_DLLS "${SFML_BIN_PATH}/*.dll")
        foreach(dll ${SFML_DLLS})
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${dll}"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
        endforeach()
    else()
        message(WARNING "SFML DLL 目录未找到，请手动复制 DLL 到输出目录")
    endif()
endif()

# 拷贝资源（方案 A：直接放到运行目录根目录）
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying resources to output directory..."
)