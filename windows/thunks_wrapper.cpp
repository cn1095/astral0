#include <windows.h>

// 使用 extern "C" 防止 C++ 名称修饰，确保符号名一致
extern "C" {

// 模拟 WaitOnAddress 函数
BOOL WINAPI WaitOnAddress(
    volatile VOID *Address,
    PVOID CompareAddress,
    SIZE_T AddressSize,
    DWORD dwMilliseconds) {
  return YY_Thunk_WaitOnAddress(Address, CompareAddress, AddressSize, dwMilliseconds);
}

// 模拟 WakeByAddressSingle 函数
VOID WINAPI WakeByAddressSingle(PVOID Address) {
  YY_Thunk_WakeByAddressSingle(Address);
}

// 模拟 WakeByAddressAll 函数
VOID WINAPI WakeByAddressAll(PVOID Address) {
  YY_Thunk_WakeByAddressAll(Address);
}

}
