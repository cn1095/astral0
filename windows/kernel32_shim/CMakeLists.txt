cmake_minimum_required(VERSION 3.10)                                    # 中文：要求最低 CMake 版本
project(kernel32_shim)                                                  # 中文：项目名

# 1. 用 kernel32_shim 作为 CMake Target 名称，避免和系统 kernel32 冲突
add_library(kernel32_shim SHARED                                       # 中文：生成一个名为 kernel32_shim 的 DLL
    shim.cpp                                                           # 中文：Shim 主源文件
    YY_Thunks_for_Win7.obj                                             # 中文：Win7 Thunks 实现对象
    exports.def                                                        # 中文：导出定义
)

# 2. 指定输出文件名为 api-ms-win-core-synch-l1-2-0.dll，这样生成的文件才叫 api-ms-win-core-synch-l1-2-0.dll
set_target_properties(kernel32_shim PROPERTIES
    OUTPUT_NAME "api-ms-win-core-synch-l1-2-0"                                             # 中文：输出文件名（去掉后缀）设为 api-ms-win-core-synch-l1-2-0
)

# 3. 告诉链接器使用 DEF 文件
set_target_properties(kernel32_shim PROPERTIES
    LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/exports.def"          # 中文：传 DEF 给链接器
)

# 4. 显式链接系统的 kernel32.lib，解析所有其他导入符号
target_link_libraries(kernel32_shim PRIVATE
    "kernel32.lib"                                                     # 中文：链接系统的 kernel32.lib 库
)

# 5. 设置输出目录到 Flutter Runner 的 bin 目录
set_target_properties(kernel32_shim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/../runner/Debug"    # 中文：Debug 输出位置
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../runner/Release"  # 中文：Release 输出位置
)
