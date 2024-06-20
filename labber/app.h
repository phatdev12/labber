#include "include/cef_app.h"

class App : public CefApp, public CefBrowserProcessHandler {
 public:
  App();

  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
    return this;
  }

  void OnContextInitialized() override;
  CefRefPtr<CefClient> GetDefaultClient() override;

 private:
  IMPLEMENT_REFCOUNTING(App);
};