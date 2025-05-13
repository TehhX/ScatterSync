# Sample Files
This folder contains testing tracking and gitignore files for use inside a personal repository (PER-R).

## .SAMPLEgitignore
This file contains the relevant ignores for files which should be ignored inside a PER-R. When actually using it, remove the "SAMPLE" inside the filename to make Git listen to its contents.

## scatterSyncCloud.bin
This file contains data which should be tracked across devices. The testing file will track a single file with generic name "File Alpha".

## scatterSyncLocal.bin
This file contains local data for a single device which Git should ignore, as seen in [.SAMPLEgitignore](#samplegitignore). The testing file will track a single file with local path "/home/". Because it is in the same order (first element) as "[...]Cloud.bin"'s "File Alpha", and will be paired during runtime.
