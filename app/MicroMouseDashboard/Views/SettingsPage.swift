import SwiftUI

struct SettingsPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
      }
      .navigationTitle("Settings")
    }
  }
}

#Preview {
  SettingsPage()
    .environmentObject(BluetoothManager())
}
