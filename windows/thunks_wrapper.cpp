// thunks_wrapper.cpp

// 声明 thunk 函数（这些函数由 YY_Thunks_for_Win7.obj 提供实现）
extern "C" void* YY_Thunk_WaitOnAddress(void* Address, void* CompareAddress, size_t AddressSize, unsigned long dwMilliseconds);
extern "C" void YY_Thunk_WakeByAddressSingle(void* Address);
extern "C" void YY_Thunk_WakeByAddressAll(void* Address);

// 替代系统 API（你也可以封装调用方式，这里只是一个基本示例）
extern "C" __declspec(dllexport) BOOL WaitOnAddress(
  volatile VOID* Address,
  PVOID CompareAddress,
  SIZE_T AddressSize,
  DWORD dwMilliseconds
) {
  return (BOOL)YY_Thunk_WaitOnAddress((void*)Address, CompareAddress, AddressSize, dwMilliseconds);
}

extern "C" __declspec(dllexport) VOID WakeByAddressSingle(PVOID Address) {
  YY_Thunk_WakeByAddressSingle(Address);
}

extern "C" __declspec(dllexport) VOID WakeByAddressAll(PVOID Address) {
  YY_Thunk_WakeByAddressAll(Address);
}
