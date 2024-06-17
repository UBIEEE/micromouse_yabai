import SwiftUI

struct ContentView: View {
  @StateObject var btManager = BluetoothManager()
  
  var body: some View {
    // Bluetooth is disabled in Settings.
    if !btManager.isBluetoothEnabled {
      BluetoothDisabledView()
    }
    // MicroMouse isn't found/connected/ready yet.
    else if !btManager.connectionState.isReady {
      NotReadyView()
        .environmentObject(btManager)
    }
    // App is ready to go!
    else {
      NavigationView()
        .environmentObject(btManager)
    }
  }
}

#Preview {
  ContentView()
}
