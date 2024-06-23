#include "Adafruit_TinyUSB.h"
#include "neopixel.h"
#include "report.h"

// data size of rgb
const int DATA_SIZE = 1284;

/* RGB */
// see readme.txt for detail
const int TapeLedDataOffset[10] = { 0, 222, 258, 294, 462, 630, 912, 948, 984, 1026 };
const int TapeLedNum[10] = { 74, 12, 12, 56, 56, 94, 12, 12, 14, 86 };
const int TapeLedPin[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

uint8_t brightness = 255;
uint8_t rgb_data[DATA_SIZE];

/* USB */
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, true);
uint8_t usb_buffer[DATA_SIZE];
// data transfer complete flag
int transfer_cplt_flag = 0;

void setup() {
  // begin watchdog
  // reset after 100ms
  rp2040.wdt_begin(1000);

  // init pio state machine for RGB
  neopixel_init(TapeLedPin[0]);

  // start HID
  #if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
    TinyUSB_Device_Init(0);
  #endif
  TinyUSBDevice.setSerialDescriptor("SDVX_RGB");                        // Set USB device serial
  TinyUSBDevice.setID(0x1234, 0x1234);                                  // Set VID, PID
  TinyUSBDevice.setProductDescriptor("sdvx RGB device");                // Set product name
  TinyUSBDevice.setManufacturerDescriptor("NEMSYS I/O SYSTEM");         // Set manufacturer name
  usb_hid.setPollInterval(1);                                           // Set 1000hz polling rate
  usb_hid.setReportCallback(get_report_callback, set_report_callback);  // set_report_callback
  usb_hid.begin();
  while (!TinyUSBDevice.mounted()) delay(1);  // wait till plugged
  while (!usb_hid.ready()) delay(1);
}

void loop() {
  // reset watchdog
  rp2040.wdt_reset();

  if (transfer_cplt_flag) {
    transfer_cplt_flag = 0;
    for (int i = 0; i < 10; i++)
    {
      uint8_t *pbase = rgb_data + TapeLedDataOffset[i];
      uint32_t buf[100];
      for (int j = 0; j < TapeLedNum[i]; j++)
      {
        buf[j] = urgb_u32(pbase[3 * j], pbase[3 * j + 1], pbase[3 * j + 2], brightness);
      }
      delayMicroseconds(300);
      neopixel_set_pin(TapeLedPin[i]);
      neopixel_set_pixels(buf, TapeLedNum[i]);
    }
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t get_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  // not used in this example
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;
  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  // This example doesn't use multiple report and report ID
  (void)report_id;
  (void)report_type;
  //usb_hid.sendReport(0, buffer, 64);
  if (buffer[0] != 20) {
    memcpy(usb_buffer + buffer[0] * 63, buffer + 1, 63);
  } else {
    memcpy(usb_buffer + buffer[0] * 63, buffer + 1, 24);
    memcpy(rgb_data, usb_buffer, DATA_SIZE);
    transfer_cplt_flag = 1;
  }
}