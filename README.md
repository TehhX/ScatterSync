# Scatter Sync
Scatter Sync is a software for synchronizing files across devices using GitHub. It allows for different local paths to be specified on different devices for the same given file. It is intended for small config, settings, or save files that may be scattered across a device. Keeping this type of file consistent across devices can be incredibly annoying.

## Table of Contents
- [Build Instructions](#build-instructions)
- [Downloading Scatter Sync](#downloading-scatter-sync)
- [Binary Data File Structures](#binary-data-file-structures)
- [Common Build Issues](#common-build-issues)
- [Reporting Bugs and Requesting New Features](#reporting-bugs-and-requesting-new-features)
- [Sample Files Folder](#sample-files-folder)
- [Development Instructions](#development-instructions)

## Build Instructions
[CMake](https://cmake.org/) is required for building this program. If using VSCode, the [CMakeTools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension is recommended, but not required. [wxWidgets](https://wxwidgets.org/) is also required for building.

After installing all prerequisites, simply build with CMake and run the resultant executable. If something goes wrong, refer to [Common Build Issues](#common-build-issues).

Git will ignore everything inside `build/` as per the .gitignore file, which is the recommended location for compiled files created with CMake.

## Downloading Scatter Sync
Refer to the [latest release](https://github.com/TehhX/ScatterSync/releases/latest) for a precompiled Linux or Windows64 executable with the latest version of Scatter Sync. They should work out of the box; just place them in the directory of your ScatterSync personal repository (the one used for file synchronization, not the one with the source code this README is inside of). From there, just follow these instructions:

* Create a new repository on GitHub
* Clone your repository locally
* Extract the archive in its entirety into your personal repo. The executable and other files should be inside the repository directly
* Launch Scatter Sync
* To add a new file to be tracked, click "Track New File" and refer to the following table:

|             |      Col. 1     |          Col. 2         |    Col. 3    |
|:-----------:|:---------------:|:-----------------------:|:------------:|
| **Content** |   Generic Name  |     Local Directory     |   File Name  |
| **Example** | My Cool File :) | C:/Users/Timmy/Desktop/ | coolfile.txt |

* Repeat installing ScatterSync on another device
* **IMPORTANT NOTE:** Delete the default scatterSyncCloud.bin file on the new device before launching ScatterSync for the first time, as Git will try to overwrite the tracked Cloud.bin file with the defualt one erasing the manifest file in the process.
* You can now specify a different local path for the same file you tracked on the first device and move or sync as before

## Binary Data File Structures
This program uses data contained within binary (.bin) files that are structured in a predefined way.

A size of "Undefined" means that it may change in length from file N to the next, but will always end in a null terminator.

All data is written directly next to one another in the order of the table from top to bottom in little endian. Examples of both binary files may be found in SampleFiles/.

### scatterSyncCloud.bin
|               Name               |                              Description                             | Size (Bytes) |
|:--------------------------------:|:--------------------------------------------------------------------:|:------------:|
|           Scroll speed           |                    Scroll speed of the file list.                    |       1      |
|         Auto sync on open        |         Should the files automatically sync with Git on open?        |      1/3*    |
|      Prompt on unpushed exit     | Should the user be prompted if they attempt to exit without pushing? |      1/3*    |
|       Auto init Git on open      |               Should GitControl be initialized on open?              |      1/3*    |
|    Identifier for file *\<N\>*   |        The identifier for the program to differentiate files.        |       2      |
|     File name for file *\<N\>*   |               The file name (not path) for a given file              |   Undefined  |
|   Generic name for file *\<N\>*  |                 User-specified name for a given file.                |   Undefined  |

*\*The three 1/3 byte sized data are contained within the same byte. They are stored in the same order as the table in bits 1-3.*

### scatterSyncLocal.bin
|               Name               |               Description               | Size (Bytes) |
|:--------------------------------:|:---------------------------------------:|:------------:|
|    Identifier for file *\<N\>*   |    To match idents with ...cloud.bin.   |       2      |
|  File directory for file *\<N\>* |  Local file directory for a given file. |   Undefined  |

## Common Build Issues
* When launching Scatter Sync on Windows, .dll files are reported missing: Make sure to move wxWidgets' .dll files next to the ScatterSync executable, inside your personal repository.

## Reporting Bugs and Requesting New Features
Use GitHub's issue system in this repo to report bugs and/or request features. If you are reporting a bug, give the steps to reproduce along with the expected and actual results of what you were trying to do when it bugs. If you're requesting a new feature, provide what the expected behavior of the new feature would be, and one or two use cases. Other than that, there are no guidelines for issue submissions.

## Sample Files Folder
The `SampleFiles/` folder contains sample binary and .gitignore files for use with ScatterSync. The contained binary files are those which come with the precompiled binaries in [releases](https://github.com/TehhX/ScatterSync/releases). The `.SAMPLEgitignore` file contains the standard ignores for ScatterSync personal repositories, and also comes with releases. When using it, it would need to be renamed to `.gitignore`, but contains "SAMPLE" so that Git ignores it in *this* repo in particular.

`SampleFiles/scatterSyncCloud.bin` contains the following default values:
* 13 scroll speed
* Don't sync on open
* Do prompt on unpushed exit
* Do auto init on open
* No file identifiers
* No generic names

<br>

`SampleFiles/scatterSyncLocal.bin` contains the following default values:
* No file identifiers
* No local paths

## Development Instructions
This includes general instructions for developing ScatterSync. They are not required whatsoever for someone who simply wants to download or build the program. They are, however, required to help develop/extend it with the intent of one's changes being eventually merged into main.

* Make sure to follow my [C++ Style Guide](https://github.com/TehhX/TehhX/blob/main/CPP_STYLEGUIDE.md)
