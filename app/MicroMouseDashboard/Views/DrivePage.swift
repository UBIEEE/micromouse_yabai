import SwiftUI

struct DrivePage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
      }
      .navigationTitle("Drive")
    }
  }
}

#Preview {
  DrivePage()
    .environmentObject(BluetoothManager())
}
