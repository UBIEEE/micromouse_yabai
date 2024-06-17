import SwiftUI

struct ContentView: View {
  @StateObject var bluetoothManager = BluetoothManager()

  var body: some View {
    if !bluetoothManager.isBluetoothEnabled {
      BluetoothDisabledView()
    } else {
      VStack {
        Image(systemName: "globe")
          .imageScale(.large)
          .foregroundStyle(.tint)
        Text("Hello, world!")
      }
      .padding()
    }
  }
}

#Preview {
  ContentView()
}
