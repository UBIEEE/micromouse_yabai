import SwiftUI

struct NotReadyView: View {
  @EnvironmentObject var btManager : BluetoothManager
  
  var body: some View {
    VStack(alignment: .leading, spacing: 5) {
      Text("\(Utilities.boolToEmoji(btManager.connectionState.isDeviceFound)) Device found")
      Text("\(Utilities.boolToEmoji(btManager.connectionState.isDeviceConnected)) Device connected")
      Text("\(Utilities.boolToEmoji(btManager.connectionState.areServicesFound)) Services found")
      Text("\(Utilities.boolToEmoji(btManager.connectionState.areCharacteristicsFound)) Characteristics found")
    }
  }
}
