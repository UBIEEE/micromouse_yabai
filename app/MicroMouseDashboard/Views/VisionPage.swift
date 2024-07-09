import SwiftUI

struct VisionPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  var body: some View {
    NavigationStack {
      List {
        Section("Raw Sensor Readings") {
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.farLeftReading)")
            Text("Left Edge")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.midLeftReading)")
            Text("Left Middle")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.midRightReading)")
            Text("Right Middle")
              .font(.subheadline)
              .foregroundColor(.secondary)
          }
          VStack(alignment: .leading) {
            Text("\(btManager.visionService.farRightReading)")
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
