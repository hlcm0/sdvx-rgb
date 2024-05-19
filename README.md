# sdvx-rgb
**output sdvx tape led data**  
![rgb_effect](https://github.com/hlcm0/sdvx-rgb/assets/103294894/ddd0477e-8d16-472d-9812-67287682c630)

## what is this:
It's a set of programs to display RGB effects of SDVX.
## it contains:
1.**modified version of bi2x_hook.cpp**: using shared memory to expose raw tape led data.  
2.**hid_send program**: reading from the shared memory and send the data to rp2040 using HID.  
3.**rp2040 firmware**: display the RGB with ws2812 light strips.
## how to use:
1.flash the firmware into rp2040 and connect the light strips to GPIO0-9 (defined in the firmware source file)  
2.connect the rp2040 to your pc and open rgb_send.exe.  
3.start the game using modified spice2x and the data should then begin to send out.  
## how to compile:
### bi2x_hook.cpp
download a 2024-05-05 version of [**spice2x**](https://github.com/spice2x/spice2x.github.io), and replace the `games\sdvx\bi2x_hook.cpp` with the one given in this repository. Compile it and it should be good to go.
### hid_send
Compile the hid_send with visual studio.
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
