#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <windows.h>

#include <string>

#include "flutter_window.h"
#include "utils.h"

extern "C" void YY_Thunks_Init();

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  const int bufferSize = 256;
  wchar_t windowTitle[bufferSize];
  
  if (GetWindowTextW(hwnd, windowTitle, bufferSize)) {
    if (_wcsicmp(windowTitle, L"Astral") == 0) {
      *((HWND*)lParam) = hwnd;
      return FALSE; 
    }
  }
  return TRUE; 
}

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev,
                      _In_ wchar_t *command_line, _In_ int show_command) {
  HANDLE hMutex = CreateMutexW(NULL, TRUE, L"AstralAppSingleInstanceMutex");
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    HWND hWnd = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&hWnd);
    
    if (hWnd != NULL) {
      if (IsIconic(hWnd)) {
        ShowWindow(hWnd, SW_RESTORE);
      }
      SetForegroundWindow(hWnd);
    }
    CloseHandle(hMutex);
    return 0;
  }

  // Attach to console when present (e.g., 'flutter run') or create a
  // new console when running with a debugger.
  if (!::AttachConsole(ATTACH_PARENT_PROCESS) && ::IsDebuggerPresent()) {
    CreateAndAttachConsole();
  }

  // Initialize COM, so that it is available for use in the library and/or
  // plugins.
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

  flutter::DartProject project(L"data");

  std::vector<std::string> command_line_arguments =
      GetCommandLineArguments();

  project.set_dart_entrypoint_arguments(std::move(command_line_arguments));

  FlutterWindow window(project);
  Win32Window::Point origin(10, 10);
  Win32Window::Size size(1280, 720);
  if (!window.Create(L"Astral", origin, size)) {
    return EXIT_FAILURE;
  }
  window.SetQuitOnClose(true);

  ::MSG msg;
  while (::GetMessage(&msg, nullptr, 0, 0)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  CloseHandle(hMutex);
  return EXIT_SUCCESS;
}
