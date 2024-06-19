#include <windows.h>

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "app.h"

#if defined(CEF_USE_SANDBOX)

#pragma comment(lib, "cef_sandbox.lib")
#endif

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  int exit_code;

#if defined(ARCH_CPU_32_BITS)
  exit_code = CefRunWinMainWithPreferredStackSize(wWinMain, hInstance,
                                                  lpCmdLine, nCmdShow);
  if (exit_code >= 0) {
    return exit_code;
  }
#endif

  void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif
  CefMainArgs main_args(hInstance);
  exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
  if (exit_code >= 0) {
    return exit_code;
  }

  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromString(::GetCommandLineW());

  CefSettings settings;
  
  settings.no_sandbox = true;
  //settings.windowless_rendering_enabled = false;

  if (command_line->HasSwitch("enable-chrome-runtime")) {
    settings.chrome_runtime = true;
  }

#if !defined(CEF_USE_SANDBOX)
  settings.no_sandbox = true;
#endif

  CefRefPtr<SimpleApp> app(new SimpleApp);

  if (!CefInitialize(main_args, settings, app.get(), sandbox_info)) {
    return 1;
  }

  CefRunMessageLoop();
  CefShutdown();

  return 0;
}
