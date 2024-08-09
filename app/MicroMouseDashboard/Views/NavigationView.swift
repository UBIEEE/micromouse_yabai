import SwiftUI

struct NavigationView: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    TabView {
      Tab("Main", systemImage: "ant") {
        MainPage()
      }
      Tab("Maze", systemImage: "map") {
        MazePage()
      }
      Tab("Settings", systemImage: "gear") {
        SettingsPage()
      }
      Tab("Error Info", systemImage: "exclamationmark.triangle") {
        ErrorInfoPage()
      }

      TabSection("Subsystems") {
        Tab("Drive", systemImage: "car.side") {
          DrivePage()
        }
        Tab("IMU", systemImage: "gauge.open.with.lines.needle.33percent") {
          IMUPage()
        }
        Tab("Vision", systemImage: "eyes") {
          VisionPage()
        }
        Tab("Music", systemImage: "music.note") {
          MusicPage()
        }
      }
    }
    // When app is ready, send a message to the MicroMouse.
    .onAppear(perform: ({
      // Don't crash the preview!
      guard Utilities.isPreviewRunning() == false else { return }
      
      let appReadyChar = btManager.connectionState.mainService.appReadyChar!
      let appReadyData = Data([1])
      btManager.writeValueToChar(appReadyChar, appReadyData)
    }))
    // Sidebar on ipadOS and macOS.
    .tabViewStyle(.sidebarAdaptable)
  }
}

#Preview {
  NavigationView()
    .environmentObject(BluetoothManager())
}
