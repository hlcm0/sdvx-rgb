const int TapeLedDataOffset[10] = { 0, 222, 258, 294, 462, 630, 912, 948, 984, 1026 };
const int TapeLedNum[10] = { 74, 12, 12, 56, 56, 94, 12, 12, 14, 86 };
const int TapeLedPin[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

/*
 * index mapping
 * 0 - title - 222 bytes - 74 colors
 * 1 - upper left speaker - 36 bytes - 12 colors
 * 2 - upper right speaker - 36 bytes - 12 colors
 * 3 - left wing - 168 bytes - 56 colors
 * 4 - right wing - 168 bytes - 56 colors
 * 5 - control panel - 282 bytes - 94 colors
 * 6 - lower left speaker - 36 bytes - 12 colors
 * 7 - lower right speaker - 36 bytes - 12 colors
 * 8 - woofer - 42 bytes - 14 colors
 * 9 - v unit - 258 bytes - 86 colors
 *
 * data is stored in RGB order, 3 bytes per color
 */