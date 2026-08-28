# justmove
Command line tool for moving filesystem items without the risk of renaming them

Was just sick of `mv` overwriting my files instead of simply moving them.  

    

Usage:  
justmove [source_path] [destination_path] [options]  
[source_path] | justmove [destination_path] [ options]  

    

`justmove` supports moving multiple source paths from `stdin` separated by newline. The new moved path is also supplied as `stdout`.  

    

```plaintext
C:\User\me\projects\test.cpp
C:\User\me\projects\test.exe
C:\User\me\projects\directory1
```
  
  
```PowerShell
(cat paths.txt) | justmove "C:\User\me\documents"; # Moves all the listed files above to C:\User\me\documents\
```
  
  
Options:  
-h --help     Show help message  
-v --verbose  Show success message after successful move operation  
  
  
Installation (Windows):  
Download the `justmove.json` file and run `scoop install .\justmove.json` in the file's directory  



Or download the executable from the Releases page https://github.com/KedFZ/justmove/releases/tag/1.3  



  
  
This project is finished and I will no longer be updating the project, although a desktop Linux-compiled binary is be welcome.
The project is licensed under the MIT License, feel free to make your own fork with the Linux binary and long-term support.
