import SwiftUI

struct ErrorInfoView: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    HStack {
      Text("RSSI: \(btManager.rssi)")
        .foregroundStyle(
            btManager.rssi >= -70 ? .green :
            btManager.rssi >= -85 ? .yellow :
            btManager.rssi >= -100 ? .orange :
            .red
        )
      Button("Read") {
        btManager.readRSSI()
      }
      .padding()
    }
    .onAppear(perform: ({
      btManager.readRSSI()
    }))
    .padding()
  }
}

#Preview {
  ErrorInfoView()
    .environmentObject(BluetoothManager())
}
