#include "handler.h"

#include <windows.h>
#include <string>

#include "include/cef_browser.h"

void SimpleHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                        const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  if (hwnd) {
    SetWindowLongPtr(hwnd, GWL_STYLE,  WS_SIZEBOX | WS_VISIBLE);
    SetWindowPos(hwnd, NULL, 0, 0, 800, 600, SWP_FRAMECHANGED | SWP_NOMOVE);
  }
}
