import SwiftUI

struct VisionView: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        Section("Far Left Sensor") {
          Text("\(btManager.visionService.farLeftReading)")
        }
        Section("Mid Left Sensor") {
          Text("\(btManager.visionService.midLeftReading)")
        }
        Section("Mid Right Sensor") {
          Text("\(btManager.visionService.midRightReading)")
        }
        Section("Far Right Sensor") {
          Text("\(btManager.visionService.farRightReading)")
        }
      }
      .navigationTitle("Vision")
    }
  }
}

#Preview {
  VisionView()
    .environmentObject(BluetoothManager())
}
