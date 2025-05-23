// windows/kernel32_shim/shim.cpp

#include <windows.h>                // 中文：包含 Windows API 头

// 以下三行无需再定义，Thunks OBJ 文件已提供实现
// extern "C" void __stdcall WaitOnAddress(void*, void*, SIZE_T, DWORD);
// extern "C" void __stdcall WakeByAddressSingle(void*);
// extern "C" void __stdcall WakeByAddressAll(void*);

BOOL APIENTRY DllMain(               // 中文：DLL 主入口函数
    HMODULE hModule,                 // 中文：模块句柄
    DWORD  ul_reason_for_call,       // 中文：调用原因
    LPVOID lpReserved                // 中文：保留参数
) {
    switch (ul_reason_for_call) {     // 中文：根据调用原因处理
        case DLL_PROCESS_ATTACH:      // 中文：进程附加时
        case DLL_THREAD_ATTACH:       // 中文：线程附加时
        case DLL_THREAD_DETACH:       // 中文：线程分离时
        case DLL_PROCESS_DETACH:      // 中文：进程分离时
            break;                    // 中文：不做额外操作
    }
    return TRUE;                     // 中文：返回成功
}
