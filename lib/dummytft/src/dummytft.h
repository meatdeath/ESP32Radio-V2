// dummytft.h
// Includes for dummy display
#ifndef DUMMYTFT_H
#define DUMMYTFT_H
#include <Arduino.h>
#define TIMEPOS     0                          // Position (column) of time in topline relative to end
#define INIPARS                                // Parameters for dsp_begin (empty)
#define DISPLAYTYPE "DUMMY"

// Color definitions for the TFT screen (if used)
#define COLOR_BLACK   0
#define COLOR_BLUE    0
#define COLOR_RED     0
#define COLOR_GREEN   0
#define COLOR_CYAN    COLOR_GREEN | COLOR_BLUE
#define COLOR_MAGENTA COLOR_RED | COLOR_BLUE
#define COLOR_YELLOW  COLOR_RED | COLOR_GREEN
#define COLOR_WHITE   COLOR_RED | COLOR_GREEN | COLOR_BLUE
#define COLOR_GREY    COLOR_RED | COLOR_GREEN | COLOR_BLUE


// Various macro's to mimic the ST7735 version of display functions
#define dsp_setRotation()                                          // Use standard landscape format
#define dsp_print(a)                                               // Print a string 
#define dsp_println(a)                                             // Print string plus newline
#define dsp_setTextSize(a)                                         // Set the text size
#define dsp_setTextColor(a)                              
#define dsp_setCursor(a,b)                                         // Position the cursor
#define dsp_erase()                                                // Clear the screen
#define dsp_getwidth()      0                                      // Get width of screen
#define dsp_getheight()     0                                      // Get height of screen
#define dsp_update(a)                                              // Updates to the physical screen
#define dsp_begin(a)        false

#define tftdata             dummytft_tftdata
#define displaybattery      dummytft_displaybattery
#define dsp_fillRect        dummytft_dsp_fillRect
#define displayvolume       dummytft_displayvolume
#define displaytime         dummytft_displaytime


struct scrseg_struct                                  // For screen segments
{
  bool     updateReq ;                               // Request update of screen
  uint16_t color ;                                    // Textcolor
  uint16_t y ;                                        // Begin of segment row
  uint16_t height ;                                   // Height of segment
  String   str ;                                      // String to be displayed
} ;



#define TFTSECS 1

extern scrseg_struct     dummytft_tftdata[TFTSECS] ;                        // Screen divided in 1 segment

void dummytft_dsp_fillRect   ( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) ;
void dummytft_displaybattery ( uint16_t bat0, uint16_t bat100, uint16_t adcval ) ;
void dummytft_displayvolume  ( uint8_t vol ) ;
void dummytft_displaytime    ( const char* str, uint16_t color = 0xFFFF ) ;

#endif

