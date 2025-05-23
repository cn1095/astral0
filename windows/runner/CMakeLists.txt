cmake_minimum_required(VERSION 3.14)
project(runner LANGUAGES CXX)

# 定义应用程序目标。要更改名称，请在顶层CMakeLists.txt中修改BINARY_NAME。
add_executable(${BINARY_NAME} WIN32
  "flutter_window.cpp"
  "main.cpp"
  "utils.cpp"
  "win32_window.cpp"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "Runner.rc"
  "runner.exe.manifest"
)

# 应用标准构建设置（可以根据需求移除）
apply_standard_settings(${BINARY_NAME})

# 定义编译宏，传递 Flutter 版本信息
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# 设置链接器参数，指定清单和子系统，兼容Windows 7
SET_TARGET_PROPERTIES(${BINARY_NAME} PROPERTIES 
  LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS"
)

# 禁用Windows宏NOMINMAX，防止和C++标准库冲突
target_compile_definitions(${BINARY_NAME} PRIVATE "NOMINMAX")

# 设置目标最低支持系统版本为Windows 7（0x0601）
target_compile_definitions(${BINARY_NAME} PRIVATE "_WIN32_WINNT=0x0601")

# ✅ 先链接 YY_Thunks_for_Win7.obj（兼容 Win7 API）
target_link_libraries(${BINARY_NAME} PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/YY_Thunks_for_Win7.obj"
)

# 链接Flutter库和应用特定库
target_link_libraries(${BINARY_NAME} PRIVATE flutter flutter_wrapper_app)

# 复制dlls目录下的DLL文件到输出目录
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/../../dlls"
    "${CMAKE_INSTALL_PREFIX}/"
  COMMENT "Copying DLL files to output directory"
  VERBATIM
)

# 链接额外Windows系统库
target_link_libraries(${BINARY_NAME} PRIVATE "dwmapi.lib")

# 添加头文件目录
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

# Flutter工具的构建依赖
add_dependencies(${BINARY_NAME} flutter_assemble)
