import CoreBluetooth

class AppConstants {
  
  class Bluetooth {
    static let MicroMouseName = "PetersMicroMouse"
    
    class MusicService {
      static let ServiceUUID = CBUUID(string: "00000000-CC7A-482A-984A-7F2ED5B3E58F");
      
      static let PlaySongUUID = CBUUID(string: "00000000-8E22-4541-9D4C-21EDAE82ED19"); // Write
      static let IsPlayingUUID = CBUUID(string: "00000001-8E22-4541-9D4C-21EDAE82ED19"); // Notify
    }
    
    class VisionService {
      static let ServiceUUID = CBUUID(string: "00000001-CC7A-482A-984A-7F2ED5B3E58F");
      
      static let DataUUID = CBUUID(string: "00000002-8E22-4541-9D4C-21EDAE82ED19"); // Notify
    }
    
    class MainService {
      static let ServiceUUID = CBUUID(string: "00000002-CC7A-482A-984A-7F2ED5B3E58F")
      
      static let SetTaskUUID = CBUUID(string: "00000003-8E22-4541-9D4C-21EDAE82ED19"); // Write
      static let CurrentTaskUUID = CBUUID(string: "00000004-8E22-4541-9D4C-21EDAE82ED19") // Notify
      static let AppReadyUUID = CBUUID(string: "00000005-8E22-4541-9D4C-21EDAE82ED19") // Write
      static let ErrorCodeUUID = CBUUID(string: "00000007-8E22-4541-9D4C-21EDAE82ED19") // Notify
    }
    
    class DriveService {
      static let ServiceUUID = CBUUID(string: "00000003-CC7A-482A-984A-7F2ED5B3E58F")
      
      static let DataUUID = CBUUID(string: "00000006-8E22-4541-9D4C-21EDAE82ED19") // Notify
      static let IMUGyroUUID = CBUUID(string: "00000008-8E22-4541-9D4C-21EDAE82ED19") // Notify
      static let IMUAccelUUID = CBUUID(string: "00000009-8E22-4541-9D4C-21EDAE82ED19") // Notify
    }
  }
}
