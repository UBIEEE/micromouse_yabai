import SwiftUI

struct MazePage: View {
  @EnvironmentObject var btManager: BluetoothManager

  var body: some View {
    NavigationStack {
      List {
      }
      .navigationTitle("Maze")
    }
  }
}

#Preview {
  MazePage()
    .environmentObject(BluetoothManager())
}
