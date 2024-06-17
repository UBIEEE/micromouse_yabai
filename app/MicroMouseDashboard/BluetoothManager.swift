import CoreBluetooth

class BluetoothManager: NSObject, ObservableObject,
                        CBCentralManagerDelegate, CBPeripheralDelegate {
  
  @Published var isBluetoothEnabled: Bool = false
  
  private var centralManager: CBCentralManager!
  private var microMouse: CBPeripheral?
  
  //
  // Connection state
  //
  
  struct ConnectionState {
    var deviceFound = false
    var deviceConnected = false
    
    struct MusicService {
      var serviceFound = false
      var playSongChar: CBCharacteristic?
      var isPlayingChar: CBCharacteristic?
      
      var isReady : Bool {
        get {
          return serviceFound && playSongChar != nil && isPlayingChar != nil
        }
      }
    }
    var musicService = MusicService()
    
    var isReady : Bool {
      get {
        return deviceFound && deviceConnected &&
               musicService.isReady
      }
    }
  }
  
  @Published var connectionState = ConnectionState()
  
  @Published var rssi = 0
  
  //
  // Music serivce
  //
  
  struct MusicService {
    var isPlaying = false
  }
  
  @Published var musicService = MusicService()
  
  //
  // Basic stuff
  //
  
  override init() {
    super.init()
    self.centralManager = CBCentralManager(delegate: self, queue: nil)
  }
  
  private func resetState() {
    connectionState = ConnectionState()
  }
  
  //
  // CBCentralManagerDelegate stuff
  //
  
  func centralManagerDidUpdateState(_ central: CBCentralManager) {
    
    if central.state == .poweredOn {
      isBluetoothEnabled = true
      centralManager.scanForPeripherals(withServices: nil)
    }
    else {
      isBluetoothEnabled = false
    }
  }
  
  func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                      advertisementData: [String: Any], rssi RSSI: NSNumber) {
    
    if peripheral.name == AppConstants.Bluetooth.MicroMouseName {
      print("MicroMouse Found!");
      microMouse = peripheral
      microMouse!.delegate = self
      centralManager.stopScan()
      centralManager.connect(peripheral)
      
      connectionState.deviceFound = true
    }
  }
  
  func centralManager(_ central: CBCentralManager,
                      didConnect peripheral: CBPeripheral) {
    
    print("MicroMouse Connected!")
    microMouse!.discoverServices(nil)
    
    connectionState.deviceConnected = true
  }
  
  func centralManager(_ central: CBCentralManager,
                      didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
    
    print("MicroMouse Disconnected!")
    resetState()
    microMouse = nil
    centralManager.scanForPeripherals(withServices: nil)
  }
  
  
  //
  // CBPeripheralDelegate stuff
  //
  
  func peripheral(_ peripheral: CBPeripheral,
                  didDiscoverServices error: Error?) {

    if let services = peripheral.services {
      for service in services {
        
        if service.uuid == AppConstants.Bluetooth.MusicService.ServiceUUID {
          peripheral.discoverCharacteristics(nil, for: service)
          
          connectionState.musicService.serviceFound = true
        }
      }
    }
    
  }

  func peripheral(_ peripheral: CBPeripheral,
                  didDiscoverCharacteristicsFor service: CBService, error: Error?) {
    
    if let characteristics = service.characteristics {
      for ch in characteristics {
        
        //
        // Music service
        //
        if ch.uuid == AppConstants.Bluetooth.MusicService.PlaySongUUID {
          connectionState.musicService.playSongChar = ch
        }
        else if ch.uuid == AppConstants.Bluetooth.MusicService.IsPlayingUUID {
          connectionState.musicService.isPlayingChar = ch
          microMouse!.setNotifyValue(true, for: ch)
        }
        //
        // Other
        //
        // ...
      }
    }
       
  }
  
  func peripheral(_ peripheral: CBPeripheral,
                  didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
    
    //
    // Music service
    //
    if characteristic.uuid == AppConstants.Bluetooth.MusicService.IsPlayingUUID {
      musicService.isPlaying = characteristic.value![0] == 1
    }
    //
    // Other
    //
    // ...
  }
  
  func peripheral(_ peripheral: CBPeripheral, didReadRSSI RSSI: NSNumber, error: Error?) {
    rssi = RSSI.intValue
  }
  
  func readRSSI() {
    microMouse?.readRSSI()
  }
  
  func writeValueToChar(_ characteristic: CBCharacteristic, _ value: Data) {
    microMouse?.writeValue(value, for: characteristic, type: .withResponse)
  }
}
