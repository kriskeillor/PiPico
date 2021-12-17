# R Pi Pico
## SDK Toolchain Setup
Required tools:  
1. [ARM GCC Compiler](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads); version 10.3-2021.10 selected; added registry information, launched gccvar.bat, added path to env var, and added registry info.
    - Destination folder: `C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10`.  
    - Readme Location: `C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\share\doc\gcc-arm-none-eabi\readme.txt`.

2. [CMake](https://cmake.org/download/); version 3.22.1 selected; added CMake to system path for all users;  
    - Destination folder: `C:\Program Files\CMake\`.  
    - Readme Location: `C:\Program Files\CMake\share\cmake-3.22\Help\index.rst`.

3. Visual Studio 2022 updated to [version 17.0.4](https://docs.microsoft.com/en-us/visualstudio/releases/2022/release-notes#17.0.4) and "C++ Build Tools core features" installed. From the Getting Started guide, the C++ build tools workload (which I could not find a description for on MS' docs) only has the Core Features and Redistributable components included. Not needing the latter update, I only did the C++ core features.

4. Python upgraded to [3.10.1](https://www.python.org/downloads/) and added to the system PATH; unable to "Install For All Users" because the option was greyed out. Disabled path character length limit.

5. Git is already installed on my system. I believe my installation commits UNIX-style, and checks out Windows-style. The Pico documentation recommends leaving both "as-is", which is the first time I as a Windows user have been advised that. Since I have projects that have been committed UNIX-style, I will not be modifying this setting.

6. I then installed C++ CMake tools for Windows *and* for Linux, which included MSVC v143 - VS 2022 C++ x86/x64 build tools (latest), C++ for Linux development, and C++ CMake tools for Linux. 

## Getting the SDK and examples
I had an interesting warning when I launched VS 2022 and opened a new "Hello World!" project - "You're using Python 0.0". I don't think so, MicroSoft, but we'll keep an eye on it. 

As part of getting the SDK, you update the submodules. I have never used [Git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) before, so this is an exciting development.

The Dev Cmd Prompt is opened from "Tools > Command Line > Developer Command Prompt". There are extensions to add a shortcut for this, but I won't install those for the time being. I set the path to the SDK as follows:
```
setx PICO_SDK_PATH "Toolchain\pico-sdk"
SUCCESS: Specified value was saved.
```

In trying to run the hello_world example, I realized the above SDK path is incorrect. I dropped the parent directory double-dots from the location. I re-ran the command with the absolute path to the SDK. I will have to redo this when I reorganize my files after graduation.

The next problem was **nmake** not being found. I re-ran VS Installer and installed C++ CMake tools, as detailed in #6 of SDK Toolchain Setup. Then I restarted my computer (for the first time in the installation process, heheh...).


## Building Hello World 
Running the example build again worked this time! I also realized I left off a double-dot from the cmake command. Not sure what that argument is for, but it made a big (positive) difference in the output. 

However, running nmake still failed. The complete output is shown in "hello-world-build-err.md". nmake is run on line 20.