import SwiftUI

@main
struct MicroMouseDashboardApp: App {
#if os(macOS)
  @NSApplicationDelegateAdaptor private var appDelegate: AppDelegate
#endif
  
  var body: some Scene {
    // Create single window for macOS app.
#if os(macOS)
    Window("MicroMouse Dashboard", id: "main") {
      ContentView()
    }
    // Normal window group for iOS app.
#else
    WindowGroup {
      ContentView()
    }
#endif
  }
}
