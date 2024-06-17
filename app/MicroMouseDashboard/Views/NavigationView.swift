import SwiftUI

struct NavigationView: View {
  var body: some View {
    TabView {
      Tab("Main", systemImage: "hare") {
        MainView()
      }
      Tab("Maze", systemImage: "map") {
        MazeView()
      }
      Tab("Settings", systemImage: "gear") {
        SettingsView()
      }
      Tab("Error Info", systemImage: "exclamationmark.triangle") {
        ErrorInfoView()
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
          Text("Music")
        }
      }
    }
    // Sidebar on ipadOS and macOS.
    .tabViewStyle(.sidebarAdaptable)
  }
}

#Preview {
  NavigationView()
}
