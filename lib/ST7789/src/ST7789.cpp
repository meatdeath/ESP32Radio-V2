//**************************************************************************************************
// ST7789.cpp                                                                                      *
//**************************************************************************************************
// Separated from the main sketch to allow several display types.                                  *
// Includes for various ST7735 displays.  Size is 240 x 240.                                       *
//**************************************************************************************************
#include "ST7789.h"

// #include <U8g2lib.h>
// #include <Arduino_GFX_Library.h>

// #define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

// /* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
// #if defined(DISPLAY_DEV_KIT)
// Arduino_GFX *gfx = create_default_Arduino_GFX();
// #else /* !defined(DISPLAY_DEV_KIT) */

// /* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
// Arduino_DataBus *bus = create_default_Arduino_DataBus();

#include <Adafruit_GFX.h>    // Core graphics library
#include "../../../src/TahomaRus.h"

Adafruit_ST7789*     ST7789_tft ;                          // For instance of display driver
scrseg_struct        ST7789_tftdata[TFTSECS] = 
{
    { .updateReq=false, .x=  0, .y=  0, .width=320, .height=32, .str="", .color=COLOR_GREY,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_TOP
    { .updateReq=false, .x=  0, .y= 30, .width=320, .height=32, .str="", .color=COLOR_CYAN,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_STATION
    { .updateReq=false, .x=100, .y=180, .width=220, .height=64, .str="", .color=COLOR_YELLOW, .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_ARTIST_SONG
    { .updateReq=false, .x=  0, .y=180, .width=320, .height=64, .str="", .color=COLOR_GREY,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_HINT
    { .updateReq=false, .x=  0, .y=  0, .width=200, .height=32, .str="", .color=COLOR_WHITE,  .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_NAME

    { .updateReq=false, .x=288, .y=  0, .width= 32, .height=32, .str="", .color=COLOR_GREY,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_WIFI,
    { .updateReq=false, .x=  0, .y=  0, .width=200, .height=32, .str="", .color=COLOR_GREY,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_BATTERY,
    { .updateReq=false, .x=  0, .y=  0, .width=200, .height=32, .str="", .color=COLOR_GREY,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_TIME,
    { .updateReq=false, .x=  0, .y=  0, .width=200, .height=32, .str="", .color=COLOR_GREY,   .backColor=COLOR_BLACK, .value=0 },   // DISP_SECTION_FAVORITE_STATION,
} ;
// scrseg_struct        ST7789_tftdata[TFTSECS] =             // Screen divided in 3 segments + 1 overlay
// {                                    // One text line is 8 pixels
//   { .updateReq = false, .x = 0, .y = 0,   .width = 200, .height = 40,  .str = "", .color = COLOR_GREY,   .background = 0x1042 },   // 1 top line
//   { .updateReq = false, .x = 0, .y = 40,  .width = 320, .height = 160, .str = "", .color = COLOR_CYAN,   .background = 0x0010 },   // 8 lines in the middle
//   { .updateReq = false, .x = 0, .y = 200, .width = 320, .height = 40,  .str = "", .color = COLOR_YELLOW, .background = COLOR_GREY },   // 4 lines at the bottom
//   { .updateReq = false, .x = 0, .y = 200, .width = 320, .height = 40,  .str = "", .color = COLOR_GREY,   .background = COLOR_GREY }    // 4 lines at the bottom for rotary encoder
// } ;


bool ST7789_dsp_begin ( int8_t cs, int8_t dc )
{
// #ifdef GFX_EXTRA_PRE_INIT
//   GFX_EXTRA_PRE_INIT();
// #endif
  if ( ( ST7789_tft = new Adafruit_ST7789 ( cs, dc , -1 ) ) )      // Create an instant for TFT
  {
    dsp_init() ;
        ST7789_tft->setFont(&TahomaRus);
        ST7789_tft->cp437(true);

        // dsp_erase();
        // dsp_setRotation();
        // dsp_fillRect(0,0,320,32,0b1100011000011000);
        // ST7789_tft->setTextColor(0x00);
        // ST7789_tft->println("ESP32 RADIO");
        // ST7789_tft->setTextColor(0xFFFF);
        // ST7789_tft->println("0123456789`~!@#$%^&*()_+-=[]\\{}|;':\",./<?>");
        // ST7789_tft->println("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        // char buf[80] = "АБВГДЕËЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
        // utf8rus2ascii(buf);
        // dsp_println(buf);
        // char buf2[80] = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
        // utf8rus2ascii(buf2);
        // dsp_println(buf2);
        // while(1);
  }
  return ( ST7789_tft != NULL ) ;
}


//**************************************************************************************************
//                                      D I S P L A Y B A T T E R Y                                *
//**************************************************************************************************
// Show the current battery charge level on the screen.                                            *
// It will overwrite the top divider.                                                              *
// No action if bat0/bat100 not defined in the preferences.                                        *
//**************************************************************************************************
void ST7789_displaybattery ( uint16_t bat0, uint16_t bat100, uint16_t adcval )
{
  if ( ST7789_tft )
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
        ypos = ST7789_tftdata[1].y - 5 ;                  // Just before 1st divider
        dsp_fillRect ( 0, ypos, newpos, 2, COLOR_GREEN ) ;      // Paint green part
        dsp_fillRect ( newpos, ypos,
                       dsp_getwidth() - newpos,
                       2, COLOR_RED ) ;                         // Paint red part
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
void ST7789_displayvolume ( uint8_t vol )
{
  if ( ST7789_tft )
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
void ST7789_displaytime ( const char* str, uint16_t color )
{
  static char oldstr[9] = "........" ;             // For compare
  uint8_t     i ;                                  // Index in strings
  uint16_t    pos = dsp_getwidth() + TIMEPOS ;     // X-position of character, TIMEPOS is negative

  if ( str[0] == '\0' )                            // Empty string?
  {
    dsp_fillRect ( 200, 0, 120, 40, tftdata[DISP_SECTION_TOP].backColor ) ;   // Clear the space for new character
    for ( i = 0 ; i < 8 ; i++ )                    // Set oldstr to dots
    {
      oldstr[i] = '.' ;
    }
    return ;                                       // No actual display yet
  }
  if ( ST7789_tft )                                // TFT active?
  {
    dsp_setTextColor ( color ) ;                   // Set the requested color
    for ( i = 0 ; i < 8 ; i++ )                    // Compare old and new
    {
      if ( str[i] != oldstr[i] )                   // Difference?
      {
        dsp_fillRect ( pos, 12, 12, 16, tftdata[DISP_SECTION_TOP].backColor ) ;   // Clear the space for new character
        dsp_setCursor ( pos, 12 ) ;                 // Prepare to show the info
        dsp_print ( str[i] ) ;                     // Show the character
        oldstr[i] = str[i] ;                       // Remember for next compare
      }
      pos += 12 ;                                  // Next position
    }
  }
}

void utf8rus2ascii(char *buf)
{
    uint16_t src_len = strlen(buf); 
    uint16_t src_i = 0;
    uint16_t dst_i = 0;

    while (src_i < src_len) 
    {
        char ch = buf[src_i++];
        if (ch >= 0xC0) 
        {
            switch (ch) 
            {
            case 0xD0:
                ch = buf[src_i++];
                if (ch == 0x81) 
                    ch = 0xA8; 
                else
                if (ch >= 0x90 && ch <= 0xBF) 
                    ch = ch + 0x30;
                break;
            
            case 0xD1:
                ch = buf[src_i++];
                if (ch == 0x91) 
                    ch = 0xB8;
                else
                if (ch >= 0x80 && ch <= 0x8F) 
                    ch = ch + 0x70;
                break;

            case 0xC3:
                ch = buf[src_i++];
                if (ch == 0x8B) ch = 0xA8;
                break;
            }
        }
        buf[dst_i++] = ch;
    }
    buf[dst_i] = '\0';
}
