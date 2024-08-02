import SwiftUI

struct MainPage: View {
  @EnvironmentObject var btManager: BluetoothManager
  
  enum Task: UInt8, CaseIterable, Identifiable {
    case mazeSearch = 1
    case mazeFastSolve = 2
    case driveStraight = 3
    
    var id: Self { self }
  }
  
  private let taskNames: [Task: String] = [
    .mazeSearch: "Maze Search",
    .mazeFastSolve: "Maze Fast Solve",
    .driveStraight: "Drive Straight (10cm)",
  ]
  private let taskDescriptions: [Task: String] = [
    .mazeSearch: "Search to the center of the maze, then back to the start",
    .mazeFastSolve: "Solve the maze as fast as possible while using previous search data",
    .driveStraight: "Drive straight for 10cm at 500mm/s",
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
            Text(taskNames[Task(rawValue: currentTask)!]!)
          }
        }
        
        Section("Error message") {
          Text("None")
        }
        
        Section(header: Text("User task Selection"), footer: Text(taskDescriptions[selectedTask]!)) {
          Picker("Task", selection: $selectedTask) {
            ForEach(Task.allCases, id: \.self) { task in
              Text(taskNames[task]!)
            }
          }
        }
        Section {
          Button("Run Task") {
            setTask(selectedTask.rawValue)
          }
        }
        Section {
          Button("Stop") {
            setTask(0)
          }
        }
      }
      .navigationTitle("MicroMouse")
    }
  }
  
  func setTask(_ task: UInt8) {
    let taskChar = btManager.connectionState.mainService.taskChar!
    let taskData = Data([task])
    btManager.writeValueToChar(taskChar, taskData)
  }
}

#Preview {
  MainPage()
    .environmentObject(BluetoothManager())
}
