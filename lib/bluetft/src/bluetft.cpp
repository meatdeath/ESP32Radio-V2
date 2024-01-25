//**************************************************************************************************
// bluetft.cpp                                                                                     *
//**************************************************************************************************
// Separated from the main sketch to allow several display types.                                  *
// Includes for various ST7735 displays.  Size is 160 x 128.  Select INITR_BLACKTAB                *
// for this and set dsp_getwidth() to 160.                                                         *
// Works also for the 128 x 128 version.  Select INITR_144GREENTAB for this and                    *
// set dsp_getwidth() to 128.                                                                      *
//**************************************************************************************************
#include "bluetft.h"

Adafruit_ST7735*     bluetft_tft ;                          // For instance of display driver
scrseg_struct        bluetft_tftdata[TFTSECS] =             // Screen divided in 3 segments + 1 overlay
                      {                                     // One text line is 8 pixels
                        { false, COLOR_WHITE,   0,  8, "" },      // 1 top line
                        { false, COLOR_CYAN,   20, 64, "" },      // 8 lines in the middle
                        { false, COLOR_YELLOW, 90, 32, "" },      // 4 lines at the bottom
                        { false, COLOR_GREEN,  90, 32, "" }       // 4 lines at the bottom for rotary encoder
                      } ;


bool bluetft_dsp_begin ( int8_t cs, int8_t dc )
{
  if ( cs < 0 || dc < 0 )
  {
    return false ;                                                  // Wrong pin configuration
  }
  if ( ( bluetft_tft = new Adafruit_ST7735 ( cs, dc , -1 ) ) )      // Create an instant for TFT
  {
    // Uncomment one of the following initR lines for ST7735R displays
    //tft->initR ( INITR_GREENTAB ) ;                               // Init TFT interface
    //tft->initR ( INITR_REDTAB ) ;                                 // Init TFT interface
    bluetft_tft->initR ( INITR_BLACKTAB ) ;                         // Init TFT interface
    //tft->initR ( INITR_144GREENTAB ) ;                            // Init TFT interface
    //tft->initR ( INITR_MINI160x80 ) ;                             // Init TFT interface
    //tft->initR ( INITR_BLACKTAB ) ;                               // Init TFT interface (160x128)
    // Uncomment the next line for ST7735B displays
    //tft_initB() ;
  }
  return ( bluetft_tft != NULL ) ;
}


//**************************************************************************************************
//                                      D I S P L A Y B A T T E R Y                                *
//**************************************************************************************************
// Show the current battery charge level on the screen.                                            *
// It will overwrite the top divider.                                                              *
// No action if bat0/bat100 not defined in the preferences.                                        *
//**************************************************************************************************
void bluetft_displaybattery ( uint16_t bat0, uint16_t bat100, uint16_t adcval )
{
  if ( bluetft_tft )
  {
    if ( bat0 < bat100 )                                  // Levels set in preferences?
    {
      static uint16_t oldpos = 0 ;                        // Previous charge level
      uint16_t        ypos ;                              // Position on screen
      uint16_t        v ;                                 // Constrainted ADC value
      uint16_t        newpos ;                            // Current setting

      v = constrain ( adcval, bat0, bat100 ) ;            // Prevent out of scale
      newpos = map ( v, bat0, bat100, 0,                  // Compute length of green bar
                     dsp_getwidth() ) ;
      if ( newpos != oldpos )                             // Value changed?
      {
        oldpos = newpos ;                                 // Remember for next compare
        ypos = bluetft_tftdata[1].y - 5 ;                 // Just before 1st divider
        dsp_fillRect ( 0, ypos, newpos, 2, COLOR_GREEN ) ;      // Paint green part
        dsp_fillRect ( newpos, ypos,
                       dsp_getwidth() - newpos,
                       2, COLOR_RED ) ;                          // Paint red part
      }
    }
  }
}


//**************************************************************************************************
//                                      D I S P L A Y V O L U M E                                  *
//**************************************************************************************************
// Show the current volume as an indicator on the screen.                                          *
// The indicator is 2 pixels heigh.                                                                *
//**************************************************************************************************
void bluetft_displayvolume ( uint8_t vol )
{
  if ( bluetft_tft )
  {
    static uint8_t oldvol = 0 ;                         // Previous volume
    uint16_t       pos ;                                // Positon of volume indicator

    if ( vol != oldvol )                                // Volume changed?
    {
      oldvol = vol ;                                    // Remember for next compare
      pos = map ( vol, 0, 100, 0, dsp_getwidth() ) ;    // Compute position on TFT
      dsp_fillRect ( 0, dsp_getheight() - 2,
                     pos, 2, COLOR_RED ) ;                    // Paint red part
      dsp_fillRect ( pos, dsp_getheight() - 2,
                     dsp_getwidth() - pos, 2, COLOR_GREEN ) ; // Paint green part
    }
  }
}


//**************************************************************************************************
//                                      D I S P L A Y T I M E                                      *
//**************************************************************************************************
// Show the time on the LCD at a fixed position in a specified color                               *
// To prevent flickering, only the changed part of the timestring is displayed.                    *
// An empty string will force a refresh on next call.                                              *
// A character on the screen is 8 pixels high and 6 pixels wide.                                   *
//**************************************************************************************************
void bluetft_displaytime ( const char* str, uint16_t color )
{
  static char oldstr[9] = "........" ;             // For compare
  uint8_t     i ;                                  // Index in strings
  uint16_t    pos = dsp_getwidth() + TIMEPOS ;     // X-position of character, TIMEPOS is negative

  if ( str[0] == '\0' )                            // Empty string?
  {
    for ( i = 0 ; i < 8 ; i++ )                    // Set oldstr to dots
    {
      oldstr[i] = '.' ;
    }
    return ;                                       // No actual display yet
  }
  if ( bluetft_tft )                               // TFT active?
  {
    dsp_setTextColor ( color ) ;                   // Set the requested color
    for ( i = 0 ; i < 8 ; i++ )                    // Compare old and new
    {
      if ( str[i] != oldstr[i] )                   // Difference?
      {
        dsp_fillRect ( pos, 0, 6, 8, COLOR_BLACK ) ;     // Clear the space for new character
        dsp_setCursor ( pos, 0 ) ;                 // Prepare to show the info
        dsp_print ( str[i] ) ;                     // Show the character
        oldstr[i] = str[i] ;                       // Remember for next compare
      }
      pos += 6 ;                                   // Next position
    }
  }
}
