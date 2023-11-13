# Ellis

Personal Game Engine project. To run this project you must first
Clone by running
```
git clone --recursive https://github.com/FacelessTiger/Ellis
```
Then go to the scripts folder and run Setup.bat. This will install and setup premake and the vulkan SDK for you. 
Finally, run the Win-GenProjects.bat to run premake and generate the project files.


Currently, project files must be created manually. This will be fixed in future versions. To do so, first make a .eproj file with the project name (eg. DNDEngine.eproj) wherever you please. In this file put in text formatted like
```
Project:
  Name: DNDEngine
  StartScene: 16921657174599981202
  AssetDirectory: "Assets"
  AssetRegistryPath: "AssetRegistry.elr"
  ScriptModulePath: "Scripts/Binaries/DNDEngine.dll"
```
Where Name is the project name, StartScene is the UUID for the start scene (we'll get to it in a second), AssetDirectory is the Assets directory location relative to the .eproj file, AssetRegistryPath is the asset registry file location relative to the Assets folder. And ScriptModulePath is the scripting DLL location relative to the Assets folder.

Once you make the Assets folder your base directory should look something like this
![image](https://github.com/FacelessTiger/Ellis/assets/24930433/912dc685-0cbd-4bc5-87c6-a322faeb7f90)

Next, in the Assets folder make the AssetRegistry file and folders and Script folders with the paths you set up in the .eproj file. I also reccomend making a seperate Scripts, Textures, and Scenes folder in the Assets folder, but it's not necessary.
Then make the start scene file wherever you wish in the Assets folder, name it whatever you wish with a .ellis extension (eg Start.ellis). In the file put this
```
Scene: Untitled
Entities:
  []
```
Now we need to specify the scene file in the AsssetRegistry, to do so put text like so in the AssetRegistry.elr file
```
AssetRegistry:
  - Handle: 16921657174599981202
    FilePath: Scenes/Start.ellis
    Type: AssetType::Scene
```
Where Handle is the UUID for the start scene, you can use the example number or any 64 bit number you wish. FilePath is the path to the start scene we made, relative to the Assets folder. And Type is just specifying the asset is a scene type.
Finally, if you make sure the UUID matches the StartScene UUID you put in the .eproj file you should be able to launch the editor and open the project! Don't worry, after this initial setup the engine takes care of updating the asset registry and making scene files and all the other nonesense for you.
