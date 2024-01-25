//**************************************************************************************************
// ST7789.h                                                                                       *
//**************************************************************************************************
// Separated from the main sketch to allow several display types.                                  *
// Includes for various ST7789 displays.  Size is 240 x 240.                                       *
//**************************************************************************************************
#ifndef ST7789_H
#define ST7789_H
#include <Adafruit_ST7789.h>

#define TIMEPOS     -104                         // Position (column) of time in topline relative to end
#define INIPARS     ini_block.tft_cs_pin, ini_block.tft_dc_pin  // Prameters for dsp_begin
#define DISPLAYTYPE "ST7789"

// Color definitions for the TFT screen (if used)
// TFT has bits 6 bits (0..5) for COLOR_RED, 6 bits (6..11) for COLOR_GREEN and 4 bits (12..15) for COLOR_BLUE.
#define COLOR_BLACK   ST77XX_BLACK
#define COLOR_RED     ST77XX_RED    // xxxx x000 0000 0000
#define COLOR_GREEN   ST77XX_GREEN  // 0000 0xxx xxx0 0000
#define COLOR_BLUE    ST77XX_BLUE   // 0000 0000 000x xxxx
#define COLOR_CYAN    (COLOR_GREEN | COLOR_BLUE)
#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)
#define COLOR_YELLOW  (COLOR_RED | COLOR_GREEN)
#define COLOR_WHITE   (COLOR_RED | COLOR_BLUE | COLOR_GREEN)
#define COLOR_GREY    ( ((ST77XX_RED>>1)&ST77XX_RED) | ((ST77XX_GREEN>>1)&ST77XX_GREEN) | (ST77XX_BLUE>>1) ) // 0x7BEF // 0111 1011 1110 1111
// #define COLOR_GREY     0x7bf0 // 0111 1011 1111 0000

// Color binary format: 0b RRRR RGGG GGGB BBBB
// Red: [0..31]
// Green: [0..63]
// Blue: [0..31]
#define COLOR_SET(R,G,B)  (((uint16_t)(R&0x1F)<<11) | ((uint16_t)(G&0x3F)<<5) | ((uint16_t)(B&0x1F)))

#define DEFTXTSIZ  1                                  // Default text size

enum {
    DISP_SECTION_TOP = 0,
    DISP_SECTION_STATION,
    DISP_SECTION_ARTIST_SONG,
    DISP_SECTION_HINT,

    DISP_SECTION_NAME,
    DISP_SECTION_WIFI,
    DISP_SECTION_BATTERY,
    DISP_SECTION_TIME,
    DISP_SECTION_FAVORITE_STATION,

    TFTSECS // Data to display.  There are TFTSECS sections
};

// // Data to display.  There are TFTSECS sections
// enum 
// {
//   TOP_SECTION = 0,
//   MIDDLE_SECTION,
//   BOTTOM_SECTION,
//   OVERLAY_SECTION,
//   TFTSECS
// };

// struct scrseg_struct                                  // For screen segments
// {
//   bool     updateReq;                                // Request update of screen
//   uint16_t color;                                    // Textcolor
//   uint16_t backColor;
//   uint16_t x;
//   uint16_t y;                                        // Begin of segment row
//   uint16_t width;
//   uint16_t height;                                   // Height of segment
//   String   str;                                      // String to be displayed
//   uint16_t value;
// } ;

struct scrseg_struct                                  // For screen segments
{
  bool     updateReq ;                               // Request update of screen
  uint16_t x ;                                        // Begin of segment row
  uint16_t y ;                                        // Begin of segment row
  uint16_t width ;                                   // Width of segment
  uint16_t height ;                                   // Height of segment
  String   str ;                                      // String to be displayed
  uint16_t color ;                                    // Textcolor
  uint16_t backColor;
  uint16_t value;
} ;

#define tftdata             ST7789_tftdata
#define displaybattery      ST7789_displaybattery
#define displayvolume       ST7789_displayvolume
#define displaytime         ST7789_displaytime

extern Adafruit_ST7789*     ST7789_tft ;                                 // For instance of display driver

// Various macro's to mimic the ST7789 version of display functions
#define dsp_setRotation()       ST7789_tft->setRotation ( 3 )            // Use landscape format (3 for upside down)
#define dsp_print(a)            ST7789_tft->print ( a )                  // Print a string 
#define dsp_println(b)          ST7789_tft->println ( b )                // Print a string followed by newline 
#define dsp_fillRect(a,b,c,d,e) ST7789_tft->fillRect ( a, b, c, d, e ) ; // Fill a rectange
#define dsp_setTextSize(a)      ST7789_tft->setTextSize(a)               // Set the text size
#define dsp_setTextColor(a)     ST7789_tft->setTextColor(a)              // Set the text color
#define dsp_setCursor(a,b)      ST7789_tft->setCursor ( a, b )           // Position the cursor
#define dsp_erase()             ST7789_tft->fillScreen ( COLOR_BLACK ) ;       // Clear the screen
#define dsp_getwidth()          320                                      // Adjust to your display
#define dsp_getheight()         240                                      // Get height of screen
#define dsp_update(a)                                                    // Updates to the physical screen
#define dsp_begin               ST7789_dsp_begin                         // Init driver
#define dsp_init()              ST7789_tft->init ( 240, 320 ) ;

extern scrseg_struct     ST7789_tftdata[TFTSECS] ;                   // Screen divided in segments
//void ST7789_dsp_fillRect   ( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) ;
void ST7789_displaybattery ( uint16_t bat0, uint16_t bat100, uint16_t adcval ) ;
void ST7789_displayvolume  ( uint8_t vol ) ;
void ST7789_displaytime    ( const char* str, uint16_t color = COLOR_BLUE ) ;
bool ST7789_dsp_begin      ( int8_t cs, int8_t dc ) ;

void utf8rus2ascii(char *source);

#endif
