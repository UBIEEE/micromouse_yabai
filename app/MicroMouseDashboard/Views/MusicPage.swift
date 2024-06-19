import SwiftUI

struct MusicPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  enum Song: UInt8, CaseIterable, Identifiable {
    case homeDepot = 4
    case nokiaRingtone = 5
    
    var id: Self { self }
  }
  
  @State private var selectedSong: Song = .homeDepot
  
  var body: some View {
    NavigationStack {
      List {
        Section("Status") {
          // Show whether the MicroMouse is currently playing anything!
          Text("\(btManager.musicService.isPlaying ? "Playing" : "Not Playing")")
        }
        Section("User Controls") {
          // Select the song to play.
          Picker("Song", selection: $selectedSong) {
            Text("Home Depot").tag(Song.homeDepot)
            Text("Nokia Ringtone").tag(Song.nokiaRingtone)
          }
          Button("\(btManager.musicService.isPlaying ? "Restart" : "Play")") {
            // Tell the MicroMouse to play the selected song.
            let playChar = btManager.connectionState.musicService.playSongChar!
            let playData = Data([selectedSong.rawValue])
            btManager.writeValueToChar(playChar, playData)
          }
          Button("Stop") {
            // Tell the MicroMouse to be quiet.
            let playChar = btManager.connectionState.musicService.playSongChar!
            let playData = Data([0])
            btManager.writeValueToChar(playChar, playData)
          }
          // Disable stop button when nothing is playing.
          .disabled(!btManager.musicService.isPlaying)
        }
      }
      .navigationTitle("Music")
    }
  }
}

#Preview {
  MusicPage()
    .environmentObject(BluetoothManager())
}
