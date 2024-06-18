import SwiftUI

struct ErrorInfoView: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        Section("RSSI") {
          Text("\(btManager.rssi)")
            .foregroundStyle(
              btManager.rssi >= -70 ? .green :
              btManager.rssi >= -85 ? .yellow :
              btManager.rssi >= -100 ? .orange :
                .red
            )
          Button("Read") {
            btManager.readRSSI()
          }
        }
      }
      .navigationTitle("Error Info")
    }
    .onAppear(perform: ({
      btManager.readRSSI()
    }))
  }
}

#Preview {
  ErrorInfoView()
    .environmentObject(BluetoothManager())
}
