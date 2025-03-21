/*
 An example analogue clock using a TFT LCD screen to show the time
 use of some of the drawing commands with the library.

 For a more accurate clock, it would be better to use the RTClib library.
 But this is just a demo.

 This sketch uses font 4 only.

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################

 Based on a sketch by Gilchrist 6/2/2014 1.0
 */

#include <SPI.h>
#define SEEED_XIAO_EPAPER_7INCH5
#include <TFT_eSPI.h> // Hardware-specific library

#ifdef EPAPER_ENABLE

EPaper epaper = EPaper(); // Invoke custom library
TFT_eSprite canvas = TFT_eSprite(&epaper);

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0; // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120; // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0; // for next 1 second timeout

static uint8_t conv2d(const char *p);                                                // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

bool initial = 1;

void setup(void)
{
  epaper.begin();
  canvas.setColorDepth(1);
  
  canvas.createSprite(240, 240);

  canvas.fillScreen(TFT_WHITE);

  canvas.setTextColor(TFT_BLACK, TFT_WHITE); // Adding a background colour erases previous text automatically

  // Draw clock face
  canvas.fillCircle(120, 120, 118, TFT_BLACK);
  canvas.fillCircle(120, 120, 110, TFT_WHITE);

  // Draw 12 lines
  for (int i = 0; i < 360; i += 30)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    x0 = sx * 114 + 120;
    yy0 = sy * 114 + 120;
    x1 = sx * 100 + 120;
    yy1 = sy * 100 + 120;

    canvas.drawLine(x0, yy0, x1, yy1, TFT_BLACK);
  }

  // Draw 60 dots
  for (int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    x0 = sx * 102 + 120;
    yy0 = sy * 102 + 120;
    // Draw minute markers
    canvas.drawPixel(x0, yy0, TFT_BLACK);

    // Draw main quadrant dots
    if (i == 0 || i == 180)
      canvas.fillCircle(x0, yy0, 2, TFT_BLACK);
    if (i == 90 || i == 270)
      canvas.fillCircle(x0, yy0, 2, TFT_BLACK);
  }

  canvas.fillCircle(120, 121, 3, TFT_BLACK);

  // Draw text at position 120,260 using fonts 4
  // Only font numbers 2,4,6,7 are valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : . - a p m
  // Font 7 is a 7 segment font and only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : .
  canvas.drawCentreString("Time flies", 120, 260, 4);

  epaper.update(0, 0, canvas.width(), canvas.height(), (uint16_t *)canvas.frameBuffer(0));

  targetTime = millis() + 1000;
}

void loop()
{
  if (targetTime < millis())
  {
    targetTime += 1000;
    ss++; // Advance second
    if (ss == 60)
    {
      ss = 0;
      mm++; // Advance minute
      if (mm > 59)
      {
        mm = 0;
        hh++; // Advance hour
        if (hh > 23)
        {
          hh = 0;
        }
      }
    }

    // Pre-compute hand degrees, x & y coords for a fast screen update
    sdeg = ss * 6;                     // 0-59 -> 0-354
    mdeg = mm * 6 + sdeg * 0.01666667; // 0-59 -> 0-360 - includes seconds
    hdeg = hh * 30 + mdeg * 0.0833333; // 0-11 -> 0-360 - includes minutes and seconds
    hx = cos((hdeg - 90) * 0.0174532925);
    hy = sin((hdeg - 90) * 0.0174532925);
    mx = cos((mdeg - 90) * 0.0174532925);
    my = sin((mdeg - 90) * 0.0174532925);
    sx = cos((sdeg - 90) * 0.0174532925);
    sy = sin((sdeg - 90) * 0.0174532925);

    if (ss == 0 || initial)
    {
      initial = 0;
      // Erase hour and minute hand positions every minute
      canvas.drawLine(ohx, ohy, 120, 121, TFT_WHITE);
      ohx = hx * 62 + 121;
      ohy = hy * 62 + 121;
      canvas.drawLine(omx, omy, 120, 121, TFT_WHITE);
      omx = mx * 84 + 120;
      omy = my * 84 + 121;
    }

    // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
    canvas.drawLine(osx, osy, 120, 121, TFT_WHITE);
    osx = sx * 90 + 121;
    osy = sy * 90 + 121;
    canvas.drawLine(osx, osy, 120, 121, TFT_BLACK);
    canvas.drawLine(ohx, ohy, 120, 121, TFT_BLACK);
    canvas.drawLine(omx, omy, 120, 121, TFT_BLACK);
    canvas.drawLine(osx, osy, 120, 121, TFT_BLACK);

    canvas.fillCircle(120, 121, 3, TFT_BLACK);
    epaper.update(0, 0, canvas.width(), canvas.height(), (uint16_t *)canvas.frameBuffer(0));
  }
}

static uint8_t conv2d(const char *p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
#else
void setup()
{

}

void loop()
{
  
}
#endif