UNREAL VERSION 5.6

## CONTROLS ##

| Action | Control | Description |
|--------|----------|-------------|
| **Move Forward/Backward** | `W / S` | Move the camera pawn forward or backward |
| **Move Left/Right** | `A / D` | Strafe camera left or right |
| **Look Around** | Move Mouse + Right Mouse Button | Rotate the camera |
| **Grab / Release** | Left Mouse Button | Pick up or drop a robot part |


## Opening the Project ##
After Cloning Repo do the following:
- Launch the RobotAbuse.uproject
- - If this fails Right-Click the RobotAbuse.uproject
- - Select Generate Visual Studio Project Files
- - Upon Generation, you should be able to Launch RobotAbuse.uproject
- - - Alternatively you can launch it in the SLN
- - - Make sure Developer Game Editor Modual is selected with Win64
- - - Run Local Windows Debugger
- - - If Error message shows, select run anyways

## In Editor ##
- Project should open at RobotAssembly Level
- - If not, You can find it in the Levels folder (do not open the AUTOTEST version)
- Press play

## Automated Functional Unit Testing ##
- In Editor top toolbar go to Tools
- Open Session Frontend
- In the new window, Find the 'Automation' Tab
- Type 'AUTOTEST' into search bar
- Checkbox both AutoTest_AttachLeftArm and AutoTest_AttachRightArm
- Press the play button in that new window (above Standard Tests)
