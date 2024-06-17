import SwiftUI

struct MainView: View {
  @EnvironmentObject var btManager: BluetoothManager

  var body: some View {
    VStack {
      Text("Main")
    }
  }
}

#Preview {
  MainView()
    .environmentObject(BluetoothManager())
}
