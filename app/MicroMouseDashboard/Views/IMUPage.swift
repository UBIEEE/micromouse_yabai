import SwiftUI

struct IMUPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        let gyroData = btManager.driveService.imuGyroData
        Section("Angular Velocity (deg/s)") {
          Text(String(format: "X: %.2f", gyroData[0]))
          Text(String(format: "Y: %.2f", gyroData[1]))
          Text(String(format: "Z: %.2f", gyroData[2]))
        }
        let accelData = btManager.driveService.imuAccelData
        Section("Linear Acceleration (Gravities)") {
          Text(String(format: "X: %.2f", accelData[0]))
          Text(String(format: "Y: %.2f", accelData[1]))
          Text(String(format: "Z: %.2f", accelData[2]))
        }
      }
      .navigationTitle("IMU")
    }
  }
}

#Preview {
  IMUPage()
    .environmentObject(BluetoothManager())
}
