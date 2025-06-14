#include "data.h"

// Note names to display
const char noteNames[72][5] = {
      " C 3", "C# 3", " D 3", "D# 3", " E 3", " F 3", "F# 3", " G 3", "G# 3", " A 3", "A# 3", " B 3",
      " C 4", "C# 4", " D 4", "D# 4", " E 4", " F 4", "F# 4", " G 4", "G# 4", " A 4", "A# 4", " B 4",
      " C 5", "C# 5", " D 5", "D# 5", " E 5", " F 5", "F# 5", " G 5", "G# 5", " A 5", "A# 5", " B 5",
      " C 6", "C# 6", " D 6", "D# 6", " E 6", " F 6", "F# 6", " G 6", "G# 6", " A 6", "A# 6", " B 6",
      " C 7", "C# 7", " D 7", "D# 7", " E 7", " F 7", "F# 7", " G 7", "G# 7", " A 7", "A# 7", " B 7",
      " C 8", "C# 8", " D 8", "D# 8", " E 8", " F 8", "F# 8", " G 8", "G# 8", " A 8", "A# 8", " B 8"
};

// GB frequencies matching notes in noteNames
// taken from http://www.devrs.com/gb/files/sndtab.html
const UWORD frequencies[] = {
  44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
  1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
  1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
  1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
  1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
  1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015
};

// the UBYTE for polynomial counter when doing notes on noise channel
const UBYTE noiseNotesFrequencies[] = {
    0X2F, 0x1F, 0x2D, 0x0F, 0x1D, 0x0D
};

// noise note name, index in the noteName array above
const int noiseNoteNameIndex[] = {
  2, 14, 20, 26, 32, 44
};

// Wave table. Different square duties to match other square channels 12.5%, 25%, 50% and 75%.
const UBYTE squareSamplesLow[] = {
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //fiddy fiddy
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00
};

// square high
const UBYTE squareSamplesHigh[] = {
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,  //fiddy fiddy
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
};

// half total wave ram length. this is so it will fit more easily
// with the frequency of the other square channels.
// the period is 16(nibbles hence 8 bytes in a row) but wave ram is 32
const UBYTE sawSamplesLow[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // silence not used
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x33, 0x33, 0x33, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00,
  0x44, 0x44, 0x33, 0x33, 0x33, 0x33, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00,
  0x55, 0x55, 0x44, 0x44, 0x44, 0x33, 0x33, 0x33, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x00, 0x00,
  0x66, 0x66, 0x55, 0x55, 0x44, 0x44, 0x44, 0x33, 0x33, 0x22, 0x22, 0x22, 0x11, 0x11, 0x00, 0x00,
  0x77, 0x77, 0x66, 0x66, 0x55, 0x55, 0x44, 0x44, 0x33, 0x33, 0x22, 0x22, 0x11, 0x11, 0x00, 0x00,
  0x88, 0x77, 0x77, 0x66, 0x66, 0x55, 0x55, 0x44, 0x44, 0x33, 0x33, 0x22, 0x22, 0x11, 0x11, 0x00,
  0x99, 0x88, 0x88, 0x77, 0x77, 0x66, 0x55, 0x55, 0x44, 0x44, 0x33, 0x22, 0x22, 0x11, 0x11, 0x00,
  0xAA, 0x99, 0x99, 0x88, 0x77, 0x77, 0x66, 0x55, 0x55, 0x44, 0x33, 0x33, 0x22, 0x11, 0x11, 0x00,
  0xBB, 0xAA, 0xAA, 0x99, 0x88, 0x77, 0x77, 0x66, 0x55, 0x44, 0x44, 0x33, 0x22, 0x11, 0x11, 0x00,
  0xCC, 0xBB, 0xAA, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x66, 0x55, 0x44, 0x33, 0x22, 0x22, 0x11, 0x00,
  0xDD, 0xCC, 0xBB, 0xAA, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x33, 0x22, 0x11, 0x00,
  0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
  0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00  // full volume
};

// saw high
const UBYTE sawSamplesHigh[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // silence not used
  0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00,
  0x33, 0x32, 0x22, 0x22, 0x11, 0x11, 0x10, 0x00,
  0x44, 0x33, 0x33, 0x22, 0x22, 0x11, 0x11, 0x00,
  0x55, 0x44, 0x43, 0x33, 0x22, 0x21, 0x11, 0x00,
  0x66, 0x55, 0x44, 0x43, 0x32, 0x22, 0x11, 0x00,
  0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
  0x87, 0x76, 0x65, 0x54, 0x43, 0x32, 0x21, 0x10,
  0x98, 0x87, 0x76, 0x55, 0x44, 0x32, 0x21, 0x10,
  0xA9, 0x98, 0x77, 0x65, 0x54, 0x33, 0x21, 0x10,
  0xBA, 0xA9, 0x87, 0x76, 0x54, 0x43, 0x21, 0x10,
  0xCB, 0xAA, 0x98, 0x76, 0x65, 0x43, 0x22, 0x10,
  0xDC, 0xBA, 0xA9, 0x87, 0x65, 0x43, 0x32, 0x10,
  0xED, 0xCB, 0xA9, 0x87, 0x76, 0x54, 0x32, 0x10,
  0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, // full volume*/
};

//samples for triangle wave LOW
const UBYTE triangleSamplesLow[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,0x00,
  0x00,0x11,0x11,0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x11,0x11,0x00,0x00,
  0x00,0x11,0x11,0x22,0x22,0x22,0x33,0x33,0x33,0x22,0x22,0x22,0x11,0x11,0x00,0x00,
  0x11,0x11,0x22,0x22,0x33,0x33,0x44,0x44,0x44,0x33,0x33,0x22,0x22,0x11,0x11,0x00,
  0x11,0x11,0x22,0x33,0x33,0x44,0x44,0x55,0x44,0x44,0x33,0x33,0x22,0x11,0x11,0x00,
  0x11,0x22,0x22,0x33,0x44,0x55,0x55,0x66,0x55,0x55,0x44,0x33,0x22,0x22,0x11,0x00,
  0x11,0x22,0x33,0x44,0x44,0x55,0x66,0x77,0x66,0x55,0x44,0x44,0x33,0x22,0x11,0x00,
  0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
  0x11,0x22,0x34,0x55,0x66,0x77,0x88,0x99,0x88,0x77,0x66,0x54,0x33,0x22,0x11,0x00,
  0x12,0x33,0x44,0x55,0x67,0x88,0x99,0xAA,0x99,0x87,0x66,0x55,0x44,0x32,0x11,0x00,
  0x12,0x33,0x45,0x66,0x77,0x89,0xAA,0xBB,0xA9,0x88,0x77,0x65,0x44,0x32,0x11,0x00,
  0x22,0x34,0x55,0x67,0x88,0x9A,0xBB,0xCC,0xBA,0x99,0x87,0x66,0x54,0x33,0x21,0x01,
  0x22,0x34,0x56,0x77,0x89,0xAA,0xBC,0xDC,0xBB,0xA9,0x88,0x76,0x54,0x33,0x21,0x01,
  0x23,0x44,0x56,0x78,0x9A,0xBB,0xCD,0xED,0xCC,0xBA,0x98,0x76,0x55,0x43,0x21,0x01,
  0x23,0x45,0x67,0x88,0x9A,0xBC,0xDE,0xFE,0xDC,0xBA,0x99,0x87,0x65,0x43,0x21,0x01,
};

// high triangle
const UBYTE triangleSamplesHigh[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x01,0x11,0x11,0x11,0x11,0x00,0x00,
  0x01,0x11,0x12,0x22,0x22,0x11,0x11,0x00,
  0x01,0x12,0x22,0x33,0x32,0x22,0x11,0x00,
  0x11,0x22,0x33,0x44,0x43,0x32,0x21,0x10,
  0x11,0x23,0x34,0x45,0x44,0x33,0x21,0x10,
  0x12,0x23,0x45,0x56,0x55,0x43,0x22,0x10,
  0x12,0x34,0x45,0x67,0x65,0x44,0x32,0x10,
  0x12,0x34,0x56,0x78,0x76,0x54,0x32,0x10,
  0x12,0x35,0x67,0x89,0x87,0x65,0x32,0x10,
  0x13,0x45,0x68,0x9A,0x98,0x65,0x43,0x10,
  0x13,0x46,0x78,0xAB,0xA8,0x76,0x43,0x10,
  0x23,0x56,0x89,0xBC,0xB9,0x86,0x53,0x20,
  0x23,0x57,0x8A,0xBD,0xBA,0x87,0x53,0x20,
  0x24,0x57,0x9B,0xCE,0xCB,0x97,0x54,0x20,
  0x24,0x68,0x9B,0xDF,0xDB,0x98,0x64,0x20,
};

// sine wave LUT
const UBYTE sineSamplesLow[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x11,0x00,0x00,0x00,0x00,0x00,0x11,
  0x22,0x22,0x33,0x33,0x33,0x33,0x33,0x22,0x22,0x11,0x00,0x00,0x00,0x00,0x00,0x11,
  0x22,0x33,0x33,0x44,0x44,0x44,0x33,0x33,0x22,0x11,0x11,0x00,0x00,0x00,0x11,0x11,
  0x33,0x33,0x44,0x55,0x55,0x55,0x44,0x33,0x33,0x22,0x11,0x00,0x00,0x00,0x11,0x22,
  0x33,0x44,0x55,0x66,0x66,0x66,0x55,0x44,0x33,0x22,0x11,0x00,0x00,0x00,0x11,0x22,
  0x44,0x55,0x66,0x77,0x77,0x77,0x66,0x55,0x43,0x22,0x11,0x00,0x00,0x00,0x11,0x23,
  0x45,0x66,0x77,0x88,0x88,0x88,0x77,0x65,0x43,0x22,0x11,0x00,0x00,0x00,0x11,0x23,
  0x55,0x67,0x88,0x99,0x99,0x99,0x87,0x66,0x54,0x32,0x11,0x00,0x00,0x00,0x11,0x23,
  0x56,0x78,0x99,0xAA,0xAA,0xAA,0x98,0x76,0x54,0x32,0x11,0x00,0x00,0x00,0x12,0x34,
  0x67,0x88,0x9A,0xBB,0xBB,0xBA,0x99,0x87,0x65,0x33,0x21,0x00,0x00,0x01,0x22,0x34,
  0x67,0x89,0xAB,0xCC,0xCC,0xCB,0xA9,0x87,0x65,0x43,0x21,0x00,0x00,0x01,0x23,0x45,
  0x78,0x9A,0xBC,0xDD,0xDD,0xDC,0xBA,0x98,0x76,0x43,0x21,0x00,0x00,0x01,0x23,0x45,
  0x78,0xAB,0xCC,0xDD,0xEE,0xDD,0xCB,0xA9,0x76,0x43,0x22,0x11,0x00,0x11,0x23,0x45,
  0x89,0xAB,0xDD,0xEE,0xFF,0xEE,0xDC,0xA9,0x87,0x54,0x22,0x11,0x00,0x11,0x23,0x56,
};

// high sine
const UBYTE sineSamplesHigh[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,
  0x11,0x22,0x22,0x21,0x11,0x00,0x00,0x01,
  0x22,0x33,0x33,0x32,0x21,0x00,0x00,0x01,
  0x23,0x34,0x44,0x33,0x21,0x10,0x00,0x11,
  0x33,0x45,0x55,0x43,0x32,0x10,0x00,0x12,
  0x34,0x56,0x66,0x54,0x32,0x10,0x00,0x12,
  0x45,0x67,0x77,0x65,0x42,0x10,0x00,0x12,
  0x46,0x78,0x88,0x76,0x42,0x10,0x00,0x12,
  0x56,0x89,0x99,0x86,0x53,0x10,0x00,0x13,
  0x57,0x9A,0xAA,0x97,0x53,0x10,0x00,0x13,
  0x68,0x9B,0xBB,0x98,0x63,0x20,0x00,0x23,
  0x68,0xAC,0xCC,0xA8,0x64,0x20,0x00,0x24,
  0x79,0xBD,0xDD,0xB9,0x74,0x20,0x00,0x24,
  0x7A,0xCD,0xED,0xCA,0x74,0x21,0x01,0x24,
  0x8A,0xDE,0xFE,0xDA,0x85,0x21,0x01,0x25,
};

// intial wave
UBYTE waveToBeLoaded[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//UBYTE waveToBeLoaded[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,};

// volume values for the "zombie" mode. https://gbdev.gg8.se/wiki/articles/Gameboy_sound_hardware#Obscure_Behavior
const UBYTE volumeValues[16] = {0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x78, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8};

// y coordinates for the volume fader(16 steps)
const UBYTE volumeFaderPosition[16] = {119, 114, 109, 104, 98, 93, 89, 85, 80, 76, 71, 67, 58, 54, 49, 41};

// bpm_in_cycles = (1024 * 60) / bpm 1-240
const uint16_t bpm_cycles_lut[240] = {
  61440, 30720, 20480, 15360, 12288, 10240, 8777, 7680, 6827, 6144,
  5585, 5120, 4726, 4389, 4096, 3840, 3614, 3413, 3234, 3072,
  2926, 2793, 2671, 2560, 2458, 2363, 2276, 2194, 2119, 2048,
  1982, 1920, 1862, 1807, 1755, 1707, 1661, 1617, 1575, 1536,
  1499, 1463, 1429, 1396, 1365, 1336, 1307, 1280, 1254, 1229,
  1205, 1182, 1159, 1138, 1117, 1097, 1078, 1059, 1041, 1024,
  1007, 991, 975, 960, 945, 931, 917, 904, 890, 878,
  865, 853, 842, 830, 819, 808, 798, 788, 778, 768,
  759, 749, 740, 731, 723, 714, 706, 698, 690, 683,
  675, 668, 661, 654, 647, 640, 633, 627, 621, 614,
  608, 602, 597, 591, 585, 580, 574, 569, 564, 559,
  554, 549, 544, 539, 534, 530, 525, 521, 516, 512,
  508, 504, 500, 495, 492, 488, 484, 480, 476, 473,
  469, 465, 462, 459, 455, 452, 448, 445, 442, 439,
  436, 433, 430, 427, 424, 421, 418, 415, 412, 410,
  407, 404, 402, 399, 396, 394, 391, 389, 386, 384,
  382, 379, 377, 375, 372, 370, 368, 366, 364, 361,
  359, 357, 355, 353, 351, 349, 347, 345, 343, 341,
  339, 338, 336, 334, 332, 330, 329, 327, 325, 323,
  322, 320, 318, 317, 315, 313, 312, 310, 309, 307,
  306, 304, 303, 301, 300, 298, 297, 295, 294, 293,
  291, 290, 288, 287, 286, 284, 283, 282, 281, 279,
  278, 277, 276, 274, 273, 272, 271, 269, 268, 267,
  266, 265, 264, 263, 261, 260, 259, 258, 257, 256
};