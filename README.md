# sdvx-rgb
**output sdvx tape led data**  
![rgb_effect](https://github.com/hlcm0/sdvx-rgb/assets/103294894/ddd0477e-8d16-472d-9812-67287682c630)

## what is this:
It's a set of programs to display RGB effects of SDVX.
## it contains:
1.**SDVXTapeLedHook.dll**: using shared memory to expose raw tape led data.  
2.**hid_send program**: reading from the shared memory and send the data to rp2040 using HID.  
3.**rp2040 firmware**: display the RGB with ws2812 light strips.
## how to use:
1.flash the firmware into rp2040 and connect the light strips to GPIO0-9. (defined in the firmware source file)  
2.connect the rp2040 to your pc and open rgb_send.exe.  
3.drag SDVXTapeLedHook.dll into the same folder as spice64.exe.  
4.configure "Inject DLL Hooks" in spicecfg. (only need to do once)  
5.start the game using spice64.exe just like normal.  
## how to compile:
### SDVXTapeLedHook.dll
1.Install MinHook static library with vcpkg
```powershell
.\vcpkg install minhook:x64-windows-static
```
2.Using Visual Studio 2022 to open the project  
3.Add to project's properties:
```
Configuration Properties -> VC++ Directories:
- Include Directories: Add $(VcpkgRoot)include
- Library Directories: Add $(VcpkgRoot)lib
```
4.Build the project
### hid_send
Compile hid_send with Visual Studio 2022.
### rp2040 firmware
1.Install **Arduino IDE** and using [**Earle Philhower's rp2040 core**](https://github.com/earlephilhower/arduino-pico).  
2.Choose TinyUSB as USB Stack and then upload the firmware.  
## reference:
- [**spice2x**](https://github.com/spice2x/spice2x.github.io)
- [**hidapi**](https://github.com/libusb/hidapi) for rgb_send
- [**arduino-pico**](https://github.com/earlephilhower/arduino-pico)
- [**pico-sdk**](https://github.com/raspberrypi/pico-sdk) for ws2812 pio assembly code
- [**Adafruit_TinyUSB_Arduino**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
- [**HID**](https://github.com/NicoHood/HID) for RawHID report descriptor
- [**MinHook**](https://github.com/TsudaKageyu/minhook)
