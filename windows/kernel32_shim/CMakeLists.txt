# windows/kernel32_shim/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)                         # 中文：要求最低 CMake 版本
project(kernel32_shim)                                       # 中文：项目名

# 添加一个共享库 target，包含 shim.cpp、Thunks OBJ 和 DEF 文件
add_library(kernel32 SHARED                                 # 中文：生成 kernel32.dll
    shim.cpp                                                # 中文：Shim 源文件
    YY_Thunks_for_Win7.obj                                  # 中文：Win7 Thunks 实现对象
    exports.def                                             # 中文：导出与转发定义
)

# 指定链接器使用 DEF 文件来控制导出/转发
set_target_properties(kernel32 PROPERTIES                   # 中文：设置 target 属性
    LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/exports.def"  # 中文：传给链接器的 DEF
)

# 设置输出目录到 Flutter Runner 可执行文件所在目录，确保 Loader 优先加载
set_target_properties(kernel32 PROPERTIES                   # 中文：再次设置 target 属性
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/../runner/Debug"   # 中文：Debug 输出
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../runner/Release" # 中文：Release 输出
)
