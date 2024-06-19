import SwiftUI

struct IMUPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
      }
      .navigationTitle("IMU")
    }
  }
}

#Preview {
  IMUPage()
    .environmentObject(BluetoothManager())
}
