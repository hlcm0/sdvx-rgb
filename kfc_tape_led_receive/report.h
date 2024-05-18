// RawHID might never work with multireports, because of OS problems
// therefore we have to make it a single report with no ID. No other HID device will be supported then.
#undef RAWHID_USAGE_PAGE
#define RAWHID_USAGE_PAGE	0xFFC0 // recommended: 0xFF00 to 0xFFFF

#undef RAWHID_USAGE
#define RAWHID_USAGE		0x0C00 // recommended: 0x0100 to 0xFFFF


uint8_t const desc_hid_report[] =
{
  	/*    RAW HID */
    0x06, lowByte(RAWHID_USAGE_PAGE), highByte(RAWHID_USAGE_PAGE),      /* 30 */
    0x0A, lowByte(RAWHID_USAGE), highByte(RAWHID_USAGE),

    0xA1, 0x01,                  /* Collection 0x01 */
    // RawHID is not multireport compatible.
    // On Linux it might work with some modifications,
    // however you are not happy to use it like that.
    //0x85, HID_REPORTID_RAWHID,			 /* REPORT_ID */
    0x75, 0x08,                  /* report size = 8 bits */
    0x15, 0x00,                  /* logical minimum = 0 */
    0x26, 0xFF, 0x00,            /* logical maximum = 255 */

    0x95, 64,        /* report count TX */
    0x09, 0x01,                  /* usage */
    0x81, 0x02,                  /* Input (array) */

    0x95, 64,        /* report count RX */
    0x09, 0x02,                  /* usage */
    0x91, 0x02,                  /* Output (array) */
    0xC0                         /* end collection */ 
};

struct rgb {
  unsigned char R;
  unsigned char G;
  unsigned char B;
};

