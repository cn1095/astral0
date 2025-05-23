// thunks_wrapper.cpp
// 仅用于强制链接 YY_Thunks_for_Win7.obj 中的符号，让 DLL 能在 Win7 上正常加载

// 声明 YY_Thunks 提供的模拟函数（无需实现，链接用）
extern "C" void YY_Thunk_WaitOnAddress();
extern "C" void YY_Thunk_WakeByAddressSingle();
extern "C" void YY_Thunk_WakeByAddressAll();

// 使用函数指针方式引用，确保链接器保留符号
typedef void (*ThunkFn)();
static ThunkFn keep1 = YY_Thunk_WaitOnAddress;
static ThunkFn keep2 = YY_Thunk_WakeByAddressSingle;
static ThunkFn keep3 = YY_Thunk_WakeByAddressAll;

// 可选 dummy 函数调用一下，保证链接成功
void ForceLinkThunks() {
    keep1();
    keep2();
    keep3();
}
