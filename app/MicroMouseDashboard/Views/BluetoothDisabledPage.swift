import SwiftUI

struct BluetoothDisabledPage: View {
  var body: some View {
    VStack(spacing: 5) {
      Text("🤯")
      Text("Bluetooth is Disabled!")
        .foregroundStyle(.red)
      Text("Fix this now!")
    }
    .padding()
  }
}

#Preview {
  BluetoothDisabledPage()
}
