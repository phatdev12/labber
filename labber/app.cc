#include "app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "handler.h"

using namespace std;

namespace {

class MainWindow : public CefWindowDelegate {
 public:
  explicit MainWindow(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

  void OnWindowCreated(CefRefPtr<CefWindow> window) override {
    window->AddChildView(browser_view_);
    window->Show();

    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow> window) override {
    browser_view_ = nullptr;
  }

  bool CanClose(CefRefPtr<CefWindow> window) override {
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser) {
      return browser->GetHost()->TryCloseBrowser();
    }
    return true;
  }

  CefSize GetPreferredSize(CefRefPtr<CefView> view) override {
    return CefSize(800, 600);
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;

  IMPLEMENT_REFCOUNTING(MainWindow);
  DISALLOW_COPY_AND_ASSIGN(MainWindow);
};

class MainBrowser : public CefBrowserViewDelegate {
 public:
  MainBrowser() {}

  bool OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view,
                                 CefRefPtr<CefBrowserView> popup_browser_view,
                                 bool is_devtools) override {
    CefWindow::CreateTopLevelWindow(
        new MainWindow(popup_browser_view));
    return true;
  }

 private:
  IMPLEMENT_REFCOUNTING(MainBrowser);
  DISALLOW_COPY_AND_ASSIGN(MainBrowser);
};

}  // namespace

App::App() {}

void App::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();



  CefRefPtr<LabberHandler> handler(new LabberHandler(false));

  CefBrowserSettings browser_settings;

  string url = "http://localhost:9000";

  CefWindowInfo window_info;
  window_info.external_begin_frame_enabled = false;
  window_info.windowless_rendering_enabled = false;
  window_info.style &= ~WS_SIZEBOX;
  
  window_info.SetAsPopup(nullptr, "Labber");

  browser_settings.background_color = CefColorSetARGB(255, 0, 0, 0);
  CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                                nullptr, nullptr);
}

CefRefPtr<CefClient> App::GetDefaultClient() {
  return LabberHandler::GetInstance();
}
