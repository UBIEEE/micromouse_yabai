import SwiftUI

struct MainPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  enum Task: UInt8, CaseIterable, Identifiable {
    case mazeSearch = 1
    case mazeFastSolve = 2
    
    var id: Self { self }
  }
  
  private let taskToString: [Task: String] = [
    .mazeSearch: "Maze Search",
    .mazeFastSolve: "Maze Fast Solve"
  ]
  
  @State private var selectedTask = Task.mazeSearch

  var body: some View {
    NavigationStack {
      List {
        Section("Current task") {
          let currentTask = btManager.mainService.currentTask
          if (currentTask == 0) {
            Text("None")
          }
          else {
            Text(taskToString[Task(rawValue: currentTask)!]!)
          }
        }
        
        Section("Error message") {
          Text("None")
        }
        
        Section("User task Selection") {
          Picker("Task", selection: $selectedTask) {
            Text("Maze Search").tag(Task.mazeSearch)
            Text("Maze Fast Solve").tag(Task.mazeFastSolve)
          }
          Button("Run") {
            let setTaskChar = btManager.connectionState.mainService.setTaskChar!
            let setTaskData = Data([selectedTask.rawValue])
            btManager.writeValueToChar(setTaskChar, setTaskData)
          }
        }
      }
      .navigationTitle("MicroMouse")
    }
  }
}

#Preview {
  MainPage()
    .environmentObject(BluetoothManager())
}
