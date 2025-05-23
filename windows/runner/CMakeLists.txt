cmake_minimum_required(VERSION 3.14)
project(runner LANGUAGES CXX)

# 设置可执行程序的名字
add_executable(${BINARY_NAME} WIN32
  "flutter_window.cpp"
  "main.cpp"
  "utils.cpp"
  "win32_window.cpp"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "Runner.rc"
  "runner.exe.manifest"
)

# 应用 flutter 的标准构建设置
apply_standard_settings(${BINARY_NAME})

# 添加版本宏定义
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# 设置管理员权限运行和窗口子系统
SET_TARGET_PROPERTIES(${BINARY_NAME} PROPERTIES LINK_FLAGS
  "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS"
)

# 禁用 Windows 冲突的宏定义
target_compile_definitions(${BINARY_NAME} PRIVATE "NOMINMAX")

# 链接 Flutter 和系统库
target_link_libraries(${BINARY_NAME} PRIVATE flutter flutter_wrapper_app "dwmapi.lib")

# 包含项目根目录
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

# === Windows 7 API 兼容性支持 ===

# 指定 thunk 模拟对象和 wrapper 源文件路径
set(YY_THUNKS_OBJ "${CMAKE_CURRENT_LIST_DIR}/../YY_Thunks_for_Win7.obj")
set(THUNKS_WRAPPER "${CMAKE_CURRENT_LIST_DIR}/../thunks_wrapper.cpp")

# 添加 wrapper 源文件（声明并强制引用模拟 API 符号）
target_sources(${BINARY_NAME} PRIVATE ${THUNKS_WRAPPER})

# 链接 thunk 对象（注入 WaitOnAddress 等函数模拟）
target_link_libraries(${BINARY_NAME} PRIVATE ${YY_THUNKS_OBJ})

# 复制 DLL 到输出目录（如 Flutter 插件依赖 DLL）
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dlls"
    "${CMAKE_INSTALL_PREFIX}/"
  COMMENT "Copying DLL files to output directory"
  VERBATIM
)

# 添加 flutter 构建依赖
add_dependencies(${BINARY_NAME} flutter_assemble)
