cmake_minimum_required(VERSION 3.14)                                                    # 中文：要求最低 CMake 版本
project(runner LANGUAGES CXX)                                                           # 中文：定义项目名和语言

# —— 新增：引入 kernel32_shim 子工程，生成兼容 Win7 的 kernel32.dll ——  
add_subdirectory(                                                                       # 中文：添加子目录
  "${CMAKE_CURRENT_LIST_DIR}/../kernel32_shim"                                          # 中文：kernel32_shim 源码路径
  "${CMAKE_CURRENT_BINARY_DIR}/kernel32_shim_build"                                     # 中文：kernel32_shim 构建输出目录
)                                                                                       # 中文：子工程添加完成
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
    # $<TARGET_FILE:kernel32_shim> 是 Shim 的全路径（C:/astral/runner/Release/kernel32.dll）
    # $<TARGET_FILE_DIR:${BINARY_NAME}> 是 Runner 可执行所在目录（build/.../runner/Release）
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:kernel32_shim>
        $<TARGET_FILE_DIR:${BINARY_NAME}>
    COMMENT "Copy kernel32.dll Shim into application output directory"
)
# 定义可执行目标。如果要修改可执行文件名，请在顶层 CMakeLists.txt 修改 BINARY_NAME  
add_executable(${BINARY_NAME} WIN32                                                     # 中文：创建 Windows GUI 可执行目标
  "flutter_window.cpp"                                                                  # 中文：Flutter 窗口封装实现
  "main.cpp"                                                                            # 中文：程序入口
  "utils.cpp"                                                                           # 中文：工具函数
  "win32_window.cpp"                                                                    # 中文：Win32 窗口实现
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"                               # 中文：Flutter 插件注册实现
  "Runner.rc"                                                                           # 中文：资源文件
  "runner.exe.manifest"                                                                 # 中文：Manifest 文件
)

# 应用标准编译设置，可根据项目需要移除  
apply_standard_settings(${BINARY_NAME})                                                 # 中文：应用 Flutter 提供的标准设置

# 为构建版本添加预处理宏  
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")   # 中文：FLUTTER_VERSION 字符串  
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}") # 中文：主版本号  
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}") # 中文：次版本号  
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}") # 中文：补丁版本号  
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}") # 中文：构建号  
set_target_properties(${BINARY_NAME} PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS")  # 中文：设置 UAC 和子系统

# 禁用与 C++ 标准库冲突的 Windows 宏  
target_compile_definitions(${BINARY_NAME} PRIVATE "NOMINMAX")                           # 中文：禁止定义 min/max 宏

# 添加依赖库和包含目录  
target_link_libraries(${BINARY_NAME} PRIVATE flutter flutter_wrapper_app)              # 中文：链接 Flutter 引擎库  
target_link_libraries(${BINARY_NAME} PRIVATE "dwmapi.lib")                             # 中文：链接 dwmapi 库  
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")               # 中文：添加当前源目录到包含路径

# 构建完成后复制 DLL 到输出目录  
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD                                    # 中文：构建后命令
  COMMAND ${CMAKE_COMMAND} -E copy_directory                                           # 中文：使用 CMake 拷贝目录
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dlls"                                          # 中文：源 DLL 目录
    "${CMAKE_INSTALL_PREFIX}/"                                                        # 中文：目标输出目录
  COMMENT "Copying DLL files to output directory"                                     # 中文：拷贝时输出注释
  VERBATIM                                                                            # 中文：精确执行命令
)

# 运行 Flutter 工具链部分，这行不能移除  
add_dependencies(${BINARY_NAME} flutter_assemble)                                      # 中文：添加与 flutter_assemble 的依赖
