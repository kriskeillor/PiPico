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

I asked a question on Stack Overflow. In the end I didn't get any direct answers, but did find some people asking [similar questions](https://stackoverflow.com/questions/14319247/cmake-is-unable-to-configure-project-for-visual-studios-10-amd64/14471934#14471934) with [more helpful answers](https://stackoverflow.com/questions/14319247/cmake-is-unable-to-configure-project-for-visual-studios-10-amd64/14471934#14471934). First I tried setting "devenv.exe" and "cl.exe" to run as administrator, which didn't work. Most of these answers were for targetting traditional platforms, so next I searched "arm m0 compile tools visual studio" which led to a [helpful blog post](https://devblogs.microsoft.com/cppblog/arm-gcc-cross-compilation-in-visual-studio/). I already had the Linux C++ workload installed, but not the Embedded and IoT development tools. 

This STILL didn't work with the Hello World example project, so I tried creating a new Pi project based on a VS template. I was told I didn't have the Linux C++ development tools. I definitely do, so there must have been some sub-component I didn't include. 

> At this point I finally noticed that I had missed a key note, "You must install the full "Windows 10 SDK" package as the SDK will need to build the pioasm and elf2uf2 tools locally.  
> Removing it from the list of installed items will mean that you will be unable to build Raspberry Pi Pico binaries."

Installing this next, hopefully following the instructions does it for me...

Installing that SDK did get me past the last issue, but I ran into a mysterious known bug associated with nmake: raspberrypi/pico-examples#152, raspberrypi/pico-examples#153. This bug is stumping the Pico developers and there is no known solution with nmake, but ninja and mingw don't cause the issue. So at this point I need to do some research and decide whether I want to go with ninja or mingw; additionally, there's the option to [use VSCode instead of Visual Studio](https://shawnhymel.com/2096/how-to-set-up-raspberry-pi-pico-c-c-toolchain-on-windows-with-vs-code/#Build_Blink_Example), which is interesting to me because it is lighter weight.

Researching different build tools, I learned how much I have to learn. Here are some resources on MinGW-w64:
- [Homepage](https://www.mingw-w64.org/)
- [Wikipedia article](https://en.wikipedia.org/wiki/Mingw-w64)

And some on Ninja:
- [Homepage](https://ninja-build.org/)
- [Wikipedia article](https://en.wikipedia.org/wiki/Ninja_%28build_system%29)

What I've learned is that these are very different tools. 

Looking at a project's neighbors can be really helpful, too. Are they tools you already use, or would like to? What I found looking at MinGW's neighbors (on the homepage) is many tools I'm not familiar with, and some I consider to be rather clunky. MinGW also wears many hats, describing itself as "a complete runtime environment for GCC & LLVM". [LLVM](https://en.wikipedia.org/wiki/LLVM) is a really cool technology for cross-referencing any language with any ISA. However, I am working on embedded development for a very specific hardware platform. I imagine LLVM could be used to cross-compile to other platforms, but there could be hardware incompatibilities, and frankly there is no pressing need. 

The list of projects using Ninja ([here](https://github.com/ninja-build/ninja/wiki/List-of-generators-producing-ninja-build-files)) is much shorter, but seems more focused on simply generating builds. This is fitting with the project's mission. Ninja has far more activity on Github, which is a site I'm active on. Ninja is also supposed to be very easy to install - simply download, add to the PATH, and go. For these reasons, I'm selecting Ninja to replace nmake. There's also a [great article on lwn.net](https://lwn.net/Articles/706404/) detailing some benefits of Ninja firsthand that is pretty reassuring. 

It's urgent that I get this working, because not only has the pico-examples project failed to build, the built in MS Visual template failed to build as well!

### Installing Ninja
Installing Ninja was as easy as downloading it from Github and adding it to the system path using "Edit the system environment variables" on Windows 10. Go to "System variables", find "Path", edit it, add a new entry, paste in the path of the Ninja executable, and you're good to go. Confirm that the setting worked by typing `$env:PATH` in Powershell. Like any command window, you have to open a new instance after editing the PATH for your changes to be reflected.

### Building with Ninja 

```
cmake -G "Ninja" .. 
ninja
```

It's that easy! It built on literally the first try. Good stuff! My Pico is blinking happily!

I appreciate the output of Ninja. It's concise and updates in real time. Some color coding would be nice, but I assume it's left out for the sake of speed.

I have to take some of the blame for how long this took (forgetting the Windows 10 SDK), but all's well that ends well!

## Exploring the Pico Examples
The [Pico Examples] repo has lots of great chunks of code to get someone started. 

For my project, I need to read from an [I2C sensor](https://www.tindie.com/products/miceuz/i2c-soil-moisture-sensor/), control a small heater and LED driver, track the time over a 24 hour cycle, and output the data over USB. Optionally, it could be displayed on a small screen, as well as sent over USB.

So, the relevant examples will demonstrate I2C control, digital outputs, and USB communication. These are: bus_scan, oled_i2c, and hello_usb. For controlling digital outputs, the PWM examples may be the best bet. I need to develop a schematic for the heater and LED peripherals before I know for sure the best control method. In all liklihood, I will use relays rather than transistors, due to high current draw on the LEDs and heaters.

To that end, these are the demos I plan on building, running, and exploring:

For using I2C peripherals:
- [x] bus_scan
- [ ] hello_pwm
- [ ] led_fade
I have been reading this [very helpful I2C series](https://rheingoldheavy.com/i2c-basics/) to gear up on I2C buses and wiring. To date I've only used prebuilt I2C circuits, never designed one myself. Unfortunately, I don't have pull-up resistors or a breadboard on hand right now, so I can't run these demos effectively.

For communicating data (not sure if I will use USB or UART): 
- [x] hello_usb
- [x] oled_i2c
- [ ] hello_uart
- [ ] uart_rx
- [ ] uart_tx
- [ ] uart_advanced

For timing:
- [x] hello_rtc  
This was fun - the original build wasn't showing up to PuTTY. I edited CMakeLists.txt to match hello_usb, enabling USB and disabling UART, as well as ensuring the target is a tinyusb_device. It was successful!  
In this process, I had to delete the *entire* examples build directory and rebuild all of them. This took several minutes (even with Ninja). I'm sure there is a way to rebuild one example at a time, but I need to figure out how...
- [ ] rtc_alarm
- [ ] rtc_alarm_repeat
- [ ] hello_timer (note: not RTC)
It is important to note that the Pico does not have a dedicated/battery backup RTC chip. These RTC examples would be reset with every power cycle.

For general digital IO:
- [ ] periodic_sampler
- [ ] dht_sensor (one candidate for humidity sensing)

For running LEDs and other pulse width modulated hardware:
- [ ] led_fade

For using the RP2040:
- [ ] hello_double_tap (will be useful for my final prototype, which likely won't have a BOOTSEL)
- [x] narrow_io_write (required reading)
- [ ] hello_multicore (with the other multicore demos to follow)

I will also add the essential code to this repo for easy referencing.

After running these demos, the next step will be exploring Programmable I/O (PIO) which may or may not be required; building RTOS for the Pico; and writing small programs that work with my sensors and drivers.

There are a lot of other cool demos I'd like to check out (SPI! Quadrature encoding!), but they're not relevant to my current project. There are also several great demos for reading data from specific sensors, not in my possession, that I will probably cannabilize code from.

## Going off on my own
Attempting to modify the narrow_io_write demo program to run over USB was giving me a lot of problems, and I could not figure out why. I decided the best thing was to minimize the number of variables. I created a new projects directory and, combining CMakeLists from the examples and [this "Learn Embedded Systems" tutorial](https://learnembeddedsystems.co.uk/pico-usb-serial-code), and code from the latter, successfully compiled a project that incorporates GPIO output (a blinking LED) and terminal output (over USB). Going forward, I intend to incorporate the pico-examples in my new HelloWorld projects folder. This should streamline the process and help me learn to develop for the Pico from scratch. It's been quite troublesome to work inside this forest that I don't know much about, so I much prefer to start from the bottom.
 
The important files to include were pico_sdk_import.cmake and the CMakeLists.txt files, which I cobbled together from the official examples and above tutorial.

Besides having CMakeLists at the top-level directory to import and initialize the sdk, specify minimum versions for CMake and the SDK, specify the c/cxx (c++?) standards for CMake, specify directories to include - it's also necessary to have CMakeLists in subdirectories if they are further organized by folder.

### Serial and USB connections
In attempting to get the narrow_io_write example working over USB, I encountered a lot of problems. Modifying the CMakeLists.txt file to enable USB did not seem to work, after many builds. I discovered that deleting simply the narrow_io_write build subdirectory and re-running Ninja would result in only that demo being recompiled, but it just wouldn't work right. In frustration, I deleted ALL the contents of the build directory and re-ran CMake and Ninja. Despite this, the build still is not appearing as a COM port after I flash it onto the Pico.

Thanks to the help of some folks over in the pi-pico-examples repo, I got some answers. I'm still not sure why the build was failing - but that problem hasn't occured again with Ninja. Regarding running the demos over USB, the issue is that once main() exits after it is finished, the USB COM link is closed. Putting an infinite loop at the end of the program prevents this, while putting the program *inside* an infinite loop allows you to actually see the output.

I also picked up a USB to TTL UART converter, the DSD Tech SH-U095C. It seems like a versatile adapter that could be useful for many projects. The wiring is not the most friendly for my Cytron Maker, so I'll pick up a grove to female pin cable next.  
The default baud rate on the Pico is 115200 bps. One handy thing about Serial as opposed to the USB COM port is that the serial port will stay open and listening throughout disconnects and power cycles on the Pico, unlike the 'virtual' (?) USB port.

### OLED example with GPIO UI
Commit #6c5bf4b
I modified the [Pico OLED example](https://github.com/raspberrypi/pico-examples/tree/master/i2c/oled_i2c) to allow the user to select a left scroll, right scroll, or no scroll by using GPIO 20, 21, and 22 as inputs. The logic levels of these pins can be manipulated easily by the Cytron Maker Pi Pico, which hooks them up to buttons (and pull-up resistors).

The modification was successful in the end. There was some difficulty in compiling owing to a mistyped type. I have been using Notepad++ to write my programs; as a text editor and not an IDE, I've been running without error checking. It took me a while to notice the error in the Ninja output, because it was reported long before the build *actually* failed. 

I did have some issues when attempting to modify the built-in art. Quite possibly, this was due to the typo above and may be corrected easily.

#### Refactoring code structure
I refactored the Pico OLED example with the functions for initializing and controlling the OLED in their own file. I'm sure I violated some best practices here, because I didn't write a header file, just put everything into a C file. I'm planning to use some (most?) of this code in a ping-pong game, with guidance from [Uri Shaked](https://hackaday.io/project/180374-pi-pico-pal-tv-pong). I just started watching his [Deep Dive into Pico and RP2040](https://hackaday.io/course/178733-raspberry-pi-pico-and-rp2040-the-deep-dive) HackadayU course, and it's been pretty illuminating so far! The highlights include:

- Mr. Shaked is an entertaining and engaged presenter
- He developed a Pico emulator in JavaScript. I'm not using it, because many of the demos he shares use Adafruit mappings, which doesn't help me much - and I have a Pico in the hardware right here, so there's no need. But, it's a very cool project and I think it will be a huge asset to Pico developers.
- His approach to reading data sheets is very illuminating. I've been reading the book "Cyberpunk" and the way the hacker Kevin Mitnick's approach to exploring new file directories (that he cracked into...) is described as an art form in and of itself, by which he deduces the most important people and files in an organization. I feel like I caught a glimpse into that kind of seasoned understanding of the 'big picture' as I watched Mr. Shaked explore the RP2040 documentation in his [first video](https://www.youtube.com/watch?v=Duel_Oaases). Some things were deemed superficial - the architecture map, for instance - that you might not expect? But I do see how the inherent shallowness of such a bird's-eye view. The register addresses were given far more time, something I am quite familiar with delving!

All in all I am really glad to have stumbled across this resource from the [embedded-related podcast](https://embedded.fm/), episode 396. I am reaching a point of diminishing returns from simply tweaking and rebuilding the examples, so I am going to watch the rest of these videos and dive into the Pong project! I already have some ideas for my own arcade games that I would love to explore in hardware *and* software.

Oh, my difficulties with CMake and Ninja continued. I'm not sure what I'm missing from my CMakeLists, but Ninja can't find "CMAKE_CXX_COMPILER" or "CMAKE_C_COMPILER". I've pored over my lists vs. the example ones, seeing few significant differences. This is my first time developing outside of an IDE so I just need to take a course or get the basics explained to me by someone more experienced.