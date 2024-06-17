import SwiftUI

struct NavigationView: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    TabView {
      Tab("Main", systemImage: "hare") {
        MainView()
          .environmentObject(btManager)
      }
      Tab("Maze", systemImage: "map") {
        MazeView()
      }
      Tab("Settings", systemImage: "gear") {
        SettingsView()
      }
      Tab("Error Info", systemImage: "exclamationmark.triangle") {
        ErrorInfoView()
          .environmentObject(btManager)
      }

      TabSection("Subsystems") {
        Tab("Drive", systemImage: "car.side") {
          Text("Drive")
        }
        Tab("IMU", systemImage: "gauge.open.with.lines.needle.33percent") {
          Text("IMU")
        }
        Tab("Vision", systemImage: "eyes") {
          Text("Vision")
        }
        Tab("Music", systemImage: "music.note") {
          MusicView()
        }
      }
    }
    // Sidebar on ipadOS and macOS.
    .tabViewStyle(.sidebarAdaptable)
  }
}

#Preview {
  NavigationView()
    .environmentObject(BluetoothManager())
}
