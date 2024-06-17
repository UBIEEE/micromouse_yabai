import SwiftUI

struct BluetoothDisabledView: View {
  var body: some View {
    VStack {
      Text("🤯")
      Text("Bluetooth is Disabled!!")
        .foregroundStyle(.red)
      Text("Fix this now!")
    }
    .padding()
  }
}
