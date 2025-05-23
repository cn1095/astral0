# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "rust_lib_astral")
project(${PROJECT_NAME} LANGUAGES CXX)

include("../cargokit/cmake/cargokit.cmake")
apply_cargokit(${PROJECT_NAME} ../../../../../../rust rust_lib_astral "" GENERATED_TARGET)
# === 添加 YY-Thunks 模拟 API 支持 ===
# 注意：路径要根据实际位置修改
set(YY_THUNKS_OBJ "${CMAKE_CURRENT_SOURCE_DIR}/../../windows/YY_Thunks_for_Win7.obj")

# 添加到 Rust 生成的库链接中
target_link_libraries(${GENERATED_TARGET} PRIVATE ${YY_THUNKS_OBJ})

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(rust_lib_astral_bundled_libraries
  "${${PROJECT_NAME}_cargokit_lib}"
  PARENT_SCOPE
)
