# Scatter Sync
Scatter Sync is a software for synchronising files across devices using GitHub. It allows for different local paths to be specified on different devices for the same given file.

It is intended for small config, settings, or save files that may be scattered across a device. Keeping this type of file consistent across devices can be incredibly annoying.

## Build Instructions
[CMake](https://cmake.org/) is required for building this program. If using VSCode, the [CMakeTools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension is reccomended, but not required. [wxWidgets](https://wxwidgets.org/) is also required for building.

After installing all prerequisites, simply build with CMake and run the resultant executable. If something goes wrong, refer to [Common Build Issues](#common-build-issues).

Git will ignore everything inside `build/` as per the .gitignore file, which is the reccomended location for compiled files created with CMake.

## Downloading Scatter Sync Executables
Refer to the [latest release](https://github.com/TehhX/ScatterSync/releases/latest) for a precompiled Linux or Windows64 executable with the latest version of Scatter Sync. They should work out of the box; just place them in the directory of your ScatterSync personal repository (the one used for file synchronization, not the one with the source code this README is inside of). From there, just follow the [End-User Instructions](#end-user-instructions).

## Binary Data File Structures
This program uses data contained within binary (.bin) files that are structured in a predefined way. A size of "Undefined" means that it may change in length from file N to the next, but will always end in a null terminator. All data is written directly next to one another in the order of the table from top to bottom in little endian. Examples of both binary files may be found in SampleFiles/.

### scatterSyncCloud.bin
|                 Name                |                              Description                             | Size (Bytes) |
|:-----------------------------------:|:--------------------------------------------------------------------:|:------------:|
|     Auto sync interval (seconds)    |          How long in seconds between auto sync. 0 for never.         |       4      |
|             Scroll speed            |                    Scroll speed of the file list.                    |       2      |
|          Auto sync on open          |         Should the files automatically sync with Git on open?        |      1/3*    |
|       Prompt on unpushed exit       | Should the user be prompted if they attempt to exit without pushing? |      1/3*    |
|        Auto init Git on open        |               Should GitControl be initialized on open?              |      1/3*    |
|    Generic name for file *\<N\>*    |                 User-specified name for a given file.                |   Undefined  |

\* The three 1/3 byte sized data are contained within the same byte. The last bit of that byte contains PoUE, the second last contains ASoO, and the third last contains AIGoO. The first 5 bits are currently unused.

### scatterSyncLocal.bin
|               Name               |                   Description                  | Size (Bytes) |
|:--------------------------------:|:----------------------------------------------:|:------------:|
|    File path for file *\<N\>*    |        Local file path for a given file.       |   Undefined  |

## End-User Instructions
These are instructions for actually using Scatter Sync, not building or compiling.

* Ceate a new repository on GitHub (private reccomended)
* Clone your repository locally wherever you want
* If building yourself, place resultant executable, .dll files, both binary files and the .SAMPLEgitignore from SampleFiles/ into your personal repo (make sure to rename ".SAMPLEgitignore" to just ".gitignore" for Git to track your files properly)
* If you're using precompiled binaries from releases, just extract the archive in its entirety into your personal repo, making sure that a subdirectory is NOT created by your archiving software, the executable and other files should be inside the repository directly
* Launch Scatter Sync via a file explorer or terminal
* Auto-init on open is checked by default, but if you turn it off you have to click "Init" after launching
* Click "Settings" to edit different settings
* To add a new file to be tracked, click "Track New File", and input its generic name for all devices to refer to it as, and local path for the device you're currently on to use
* Make sure the local path contains the file name and extension as well e.g. `C:/Users/USERNAME/Desktop/file.txt`
* Click "Move to Repo" to move all your tracked files to the repo, or "Move to Local" to move all your tracked files to their local paths
* Click "Sync" to get updates to and from Git. Make sure that all files are inside the repo if you want to sync them
* If you want to make changes to the repo itself, you can terminal cd into the repo and use Git as normal, or interface from GitHub.com itself
* Repeat installing ScatterSync on another device
* You can now specify a different local path for the same file you tracked on the first device and move or sync as normal

## Common Build Issues
* When launching Scatter Sync on Windows, .dll files are reported missing: Make sure to move wxWidgets' .dll files next to ScatterSync.[exe, out].

## Sample Files Folder
The `SampleFiles/` folder contains sample binary and .gitignore files for use with ScatterSync. `SampleFiles/ExampleFiles/` contains example files with one tracked file to be examined in a hex editor to learn more about the binary files. `SampleFiles/DefaultFiles` contains the default binary files that come with the precompiled binaries in [releases](https://github.com/TehhX/ScatterSync/releases). The `.SAMPLEgitignore` file contains the standard ignores for ScatterSync personal repositories, and also comes with releases. When using it, it would need to be renamed to `.gitignore`, but contains "SAMPLE" so that Git ignores it in *this* repo in particular.
