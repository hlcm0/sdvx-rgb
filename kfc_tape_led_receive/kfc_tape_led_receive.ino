#include "Adafruit_TinyUSB.h"
#include <Adafruit_NeoPixel.h>
#include "report.h"

const int TapeLedDataOffset[10] = { 0, 222, 258, 294, 462, 630, 912, 948, 984, 1026 };
const int TapeLedNum[10] = { 74, 12, 12, 56, 56, 94, 12, 12, 14, 86 };
const int TapeLedPin[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, true);

// data size of rgb
const int DATA_SIZE = 1284;

uint8_t usb_buffer[DATA_SIZE];
uint8_t rgb_data[DATA_SIZE];
// data transfer complete flag
int transfer_cplt_flag = 0;

Adafruit_NeoPixel tt(TapeLedNum[0], TapeLedPin[0], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel uls(TapeLedNum[1], TapeLedPin[1], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel urs(TapeLedNum[2], TapeLedPin[2], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel lw(TapeLedNum[3], TapeLedPin[3], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rw(TapeLedNum[4], TapeLedPin[4], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel cp(TapeLedNum[5], TapeLedPin[5], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel lls(TapeLedNum[6], TapeLedPin[6], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel lrs(TapeLedNum[7], TapeLedPin[7], NEO_GRB + NEO_KHZ800);

void setup() {
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
  tt.begin();
  uls.begin();
  urs.begin();
  lw.begin();
  rw.begin();
  cp.begin();
  lls.begin();
  lrs.begin();
}

unsigned long last_time=0;
void loop() {
  if (transfer_cplt_flag) {
    transfer_cplt_flag = 0;
    int k;
    k=0;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      tt.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    tt.show();
    k=1;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      uls.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    uls.show();
    k=2;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      urs.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    urs.show();
    k=3;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      lw.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    lw.show();
    k=4;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      rw.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    rw.show();
    k=5;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      cp.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    cp.show();
    k=6;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      lls.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    lls.show();
    k=7;
    for (int i=0;i<TapeLedNum[k];i++)
    {
      lrs.setPixelColor(i, rgb_data[TapeLedDataOffset[k]+3*i], rgb_data[TapeLedDataOffset[k]+3*i+1], rgb_data[TapeLedDataOffset[k]+3*i+2]);
    }
    lrs.show();
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