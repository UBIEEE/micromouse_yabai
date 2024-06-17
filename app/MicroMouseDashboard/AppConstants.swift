import CoreBluetooth

class AppConstants {
  
  class Bluetooth {
    static let MicroMouseName = "PetersMicroMouse"
    
    class MusicService {
      static let ServiceUUID = CBUUID(string: "00000000-CC7A-482A-984A-7F2ED5B3E58F");
      
      static let PlaySongUUID = CBUUID(string: "00000000-8E22-4541-9D4C-21EDAE82ED19"); // Write
      static let IsPlayingUUID = CBUUID(string: "00000001-8E22-4541-9D4C-21EDAE82ED19"); // Notify
    }
  }
}
