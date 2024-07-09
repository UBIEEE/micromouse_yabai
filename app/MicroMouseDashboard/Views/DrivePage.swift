import SwiftUI

struct DrivePage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        Section("Left") {
          VStack(alignment: .leading) {
            Text("\(btManager.driveService.motorLeftPosition)")
            Text("Position (mm)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
          Text("\(btManager.driveService.motorLeftVelocity)")
            Text("Velocity (mm/s)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
        }
        Section("Right") {
          VStack(alignment: .leading) {
            Text("\(btManager.driveService.motorRightPosition)")
            Text("Position (mm)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.driveService.motorRightVelocity)")
            Text("Velocity (mm/s)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
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
