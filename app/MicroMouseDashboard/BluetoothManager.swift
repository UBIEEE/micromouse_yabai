import CoreBluetooth

class BluetoothManager: NSObject, CBCentralManagerDelegate, ObservableObject {
  @Published var isBluetoothEnabled: Bool = false
  @Published var microMouseManager = MicroMouseManager()

  private var centralManager: CBCentralManager!
  
  override init() {
    super.init()
    self.centralManager = CBCentralManager(delegate: self, queue: nil)
  }
  
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
      microMouseManager.setPeripheral(peripheral)
      centralManager.stopScan()
      centralManager.connect(peripheral)
    }
  }
  
  func centralManager(_ central: CBCentralManager,
                      didConnect peripheral: CBPeripheral) {
    
    print("MicroMouse Connected!")
    microMouseManager.microMouse!.discoverServices(nil)
  }
  
  func centralManager(_ central: CBCentralManager,
                      didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
    
    print("MicroMouse Disconnected!")
    microMouseManager.setPeripheral(nil)
    centralManager.scanForPeripherals(withServices: nil)
  }
}
