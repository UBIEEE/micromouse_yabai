import SwiftUI

struct DrivePage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        Section("Left") {
          Text("\(btManager.driveService.motorLeftPosition)  mm")
          Text("\(btManager.driveService.motorLeftVelocity)  mm/s")
        }
        Section("Right") {
          Text("\(btManager.driveService.motorRightPosition)  mm")
          Text("\(btManager.driveService.motorRightVelocity)  mm/s")
        }
      }
      .navigationTitle("Drive")
    }
  }
}

#Preview {
  DrivePage()
    .environmentObject(BluetoothManager())
}
