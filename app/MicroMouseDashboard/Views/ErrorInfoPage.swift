import SwiftUI

struct ErrorInfoPage: View {
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
        Section("Errors") {
          ForEach(btManager.mainService.errorCodes, id: \.self) { errorCode in
            Text("\(errorCode)")
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
  ErrorInfoPage()
    .environmentObject(BluetoothManager())
}
