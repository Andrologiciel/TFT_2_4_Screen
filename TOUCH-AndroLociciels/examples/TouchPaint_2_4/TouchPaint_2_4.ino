//-- Andrologiciels TouchPaint example for TFT 2.4 '' ------------//
// with Spfd5408 controller                                  //
// (c) AndroLogiciels 2015                                   //
///////////////////////////////////////////////////////////////
// You can 'calibrate' the screen with CalX and CalY values  //
// Multimeter X+ (Pin 6) & X- (pin A2) = 'Sens'itivity value // 
///////////////////////////////////////////////////////////////
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>     // Touch library
//-- Calibrates value
#define CalX 1
#define CalY -2
#define Sens 750
#define MINPRESSURE 10
#define MAXPRESSURE 1000
//240x320
//-- These are the pins for the shield!
#define YP A1 //A1
#define XM A2 //A2
#define YM 7   //7
#define XP 6   //6
//-- Screen coordinates 240 x 320 resolution 
#define TS_MINX  150
#define TS_MINY  120
#define TS_MAXX  920
#define TS_MAXY  940
//-- Init TouchScreen :
TouchScreen ts = TouchScreen(XP, YP, XM, YM, Sens);
//-- LCD Pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4 // Optional : otherwise connect to Arduino's reset pin
//-- Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	RED     0x001F
#define	BLUE    0xF800 
#define	GREEN   0x07E0
#define CYAN    0xFFE0 
#define MAGENTA 0xF81F
#define YELLOW  0x07FF
#define WHITE   0xFFFF
//-- Init LCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define BOXSIZE   40  //-- Palette width
#define PENRADIUS  3  //-- The 'pencil' size
int oldcolor, currentcolor;
//-- Setup
void setup(void) {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);
  Serial.println(F("TFT LCD test"));
  Serial.print("TFT size is "); 
  Serial.print(tft.width()); 
  Serial.print("x"); 
  Serial.println(tft.height());
  Serial.println(F("Paint!"));
  tft.fillScreen(BLACK);

  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE); 
  currentcolor = RED;
  
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      
    // scale from 0->1023 to tft.width  
    //Original code, contain bug (inverted coordinate)   
    //Code below to fix the bug of inverted coordinates
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height()  );
    
    //Correct offset of touch. Manual calibration
    p.x+=CalX;
    p.y+=CalY;
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    Serial.print("tPressure = "); 
    Serial.println(p.z);

    if (p.y < BOXSIZE) {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) { 
         currentcolor = RED; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = YELLOW;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = GREEN;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = CYAN;
         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = MAGENTA;
         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
       }
       if (oldcolor != currentcolor) {
          if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
       }
    }
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
  }
}
