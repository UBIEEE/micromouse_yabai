import SwiftUI

struct ConnectionStatusPage: View {
  @EnvironmentObject var btManager : BluetoothManager
  
  var body: some View {
    VStack {
      Text("Connection Status")
        .font(.title)
      
      VStack(alignment: .leading, spacing: 5) {
        Text("\(Utilities.boolToEmoji(btManager.connectionState.deviceFound)) Device found")
        Text("\(Utilities.boolToEmoji(btManager.connectionState.deviceConnected)) Device connected")
        Text("\(Utilities.boolToEmoji(btManager.connectionState.mainService.isReady)) Main Service ready")
        Text("\(Utilities.boolToEmoji(btManager.connectionState.musicService.isReady)) Music Service ready")
        Text("\(Utilities.boolToEmoji(btManager.connectionState.visionService.isReady)) Vision Service ready")
      }
      .padding()
    }
    .padding()
  }
}

#Preview {
  ConnectionStatusPage()
    .environmentObject(BluetoothManager())
}
