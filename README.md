# sdvx-rgb
output sdvx tape led data
## what is this:
It's a set of programs to display RGB effects of SDVX.
## it contains:
1.**modified version of bi2x_hook.cpp**: using shared memory to expose raw tape led data.  
2.**hid_send program**: reading from the shared memory and send the data to rp2040 using HID.  
3.**rp2040 firmware**: display the RGB with ws2812 light strips.
## how to use:
### bi2x_hook.cpp
download a 2024-05-05 version of [**spice2x**](https://github.com/spice2x/spice2x.github.io), and replace the `games\sdvx\bi2x_hook.cpp` with the one given in this repository. Compile it and it should be good to go.
### hid_send
Compile the hid_send with visual studio.
### rp2040 firmware
1.Install **Arduino IDE** and using [**Earle Philhower's rp2040 core**](https://github.com/earlephilhower/arduino-pico).  
2.Install **Adafruit Neopixel** library from library manager.  
3.Choose TinyUSB as USB Stack and then upload the firmware.  
## some notes:
- Currently the rp2040 firmware only displays 8 strips rather than the total 10. This is because the Adafruit_Neopixel only support up to 8 strips on RP2040(limited by the number of pio state machines), and I couldn't find a way to change the sideset pin of a pio state machine on the fly. If anyone knows a way to support 10 strips on RP2040, please let me know. Thanks!
## special thanks:
- [**spice2x**](https://github.com/spice2x/spice2x.github.io)
- [**hidapi**](https://github.com/libusb/hidapi)
- [**arduino-pico**](https://github.com/earlephilhower/arduino-pico)
- [**Adafruit_Neopixel**](https://github.com/adafruit/Adafruit_NeoPixel)
- [**Adafruit_TinyUSB_Arduino**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
