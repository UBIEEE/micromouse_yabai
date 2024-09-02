import SwiftUI

struct VisionPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        Section("Normalized Sensor Readings") {
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.farLeftReading)")
            Text("Left Edge (mm)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.midLeftReading)")
            Text("Left Middle (mm)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.midRightReading)")
            Text("Right Middle (mm)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.farRightReading)")
            Text("Right Edge (mm)")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
        }
        Section("Raw Sensor Readings") {
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.rawFarLeftReading)")
            Text("Left Edge")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.rawMidLeftReading)")
            Text("Left Middle")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.rawMidRightReading)")
            Text("Right Middle")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.rawFarRightReading)")
            Text("Right Edge")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
        }
      }
      .navigationTitle("Vision")
    }
  }
}

#Preview {
  VisionPage()
    .environmentObject(BluetoothManager())
}
