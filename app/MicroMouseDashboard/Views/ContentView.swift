import SwiftUI

struct ContentView: View {
  @StateObject var btManager = BluetoothManager()
  
  var body: some View {
    if !btManager.isBluetoothEnabled {
      BluetoothDisabledView()
    }
    else if !btManager.connectionState.isReady {
      NotReadyView()
        .environmentObject(btManager)
    }
    else {
      VStack {
        Text("Connected!!")
      }
      .padding()
    }
  }
}

#Preview {
  ContentView()
}
