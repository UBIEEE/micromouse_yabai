import CoreBluetooth

//
// Delegate class for communication with the MicroMouse.
//
class MicroMouseManager: NSObject, CBPeripheralDelegate, ObservableObject {
  var microMouse: CBPeripheral?
  
  
  func setPeripheral(_ peripheral: CBPeripheral?) {
    microMouse = peripheral
    if microMouse != nil {
      microMouse?.delegate = self
    }
  }
  
  func peripheral(_ peripheral: CBPeripheral,
                  didDiscoverServices error: Error?) {

    if let services = peripheral.services {
      for service in services {
        if service.uuid == AppConstants.Bluetooth.ServiceUUID {
          print("Found the service!")
          peripheral.discoverCharacteristics([AppConstants.Bluetooth.CharUUID], for: service)
        }
      }
    }
    
  }

  func peripheral(_ peripheral: CBPeripheral,
                  didDiscoverCharacteristicsFor service: CBService, error: Error?) {
    
    if let characteristics = service.characteristics {
      for ch in characteristics {
        if ch.uuid == AppConstants.Bluetooth.CharUUID {
          print("Found the characteristic!")
        }
      }
    }
       
  }
  
  func peripheral(_ peripheral: CBPeripheral,
                  didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
    
    print("Updated value!")
  }
}
