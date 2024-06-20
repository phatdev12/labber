#include <windows.h>

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "app.h"

HINSTANCE hInst;

#if defined(CEF_USE_SANDBOX)

#pragma comment(lib, "cef_sandbox.lib")
#endif

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  hInst = hInstance;
  WNDCLASSEX wcex;
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  RegisterClassEx(&wcex);
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  int exit_code;


  void* sandbox_info = nullptr;

  CefMainArgs main_args(hInstance);
  
  exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
  if (exit_code >= 0) return exit_code;

  CefSettings settings;
  
  settings.no_sandbox = true;
  //settings.windowless_rendering_enabled = false;

  CefRefPtr<App> app(new App);

  CefInitialize(main_args, settings, app.get(), NULL);

  CefRunMessageLoop();
  CefShutdown();

  return 0;
}
