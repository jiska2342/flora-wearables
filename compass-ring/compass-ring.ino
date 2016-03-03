/*
  Takes compass measurement and displays it on a neopixel ring.
*/
#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIXEL_PIN   6  //pin for neopixel ring
#define PIXEL_COUNT 12 //ring size
#define Pi          3.14159

Adafruit_LSM303 lsm;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
  //Debugging output including detailled positioning information
  Serial.begin(9600);

  // Try to initialise  and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }


  strip.begin(); //initialize pixels
  strip.setBrightness(50);
}

void loop()
{
  lsm.read();
  setDirection(getDirection(lsm.magData.x, lsm.magData.y));


  delay(1000);
}

/*
  show direction in neopixel ring
*/
void setDirection(float heading)
{
  int pos = (int) (heading / 360 * 12);
  Serial.println(pos);

  //loop through the whole ring, set colors depending on position
  for (int i = 0; i < PIXEL_COUNT; i++) {

    int center = i - pos;
    if (center < 0) {
      center += PIXEL_COUNT;
    }
    strip.setPixelColor(i, getRingColor(PIXEL_COUNT, center, i));
  }
  strip.show(); //send updates to pixel ring
}


/*
  generate ring with consistent colors but a brighter towards the compass direction
*/
uint32_t getRingColor(int pixels, int brightnessPosition, int colorPosition)
{
  uint32_t color;

  //generate color depending on pixel number
  byte offset = (byte) (colorPosition * 255 / pixels);

  //make center pixel the brightest via parabola function
  float widthFactor = pixels * 1.5;
  float brightness = (-pow(brightnessPosition - (pixels / 2), 2) + widthFactor) / widthFactor;

  if (brightness < 0) {
    brightness = 0;
  }

  //255:3(r,g,b)=85
  if (offset < 85) {
    color = strip.Color((255 - offset * 3) * brightness, 0, offset * 3 * brightness);
    return color;
  }
  if (offset < 170) {
    offset -= 85;
    color = strip.Color(0, offset * 3 * brightness, (255 - offset * 3) * brightness);
    return color;
  }
  offset -= 170;
  color = strip.Color(offset * 3 * brightness, (255 - offset * 3) * brightness, 0);


  return color;
}

/* the magnetic field is measured in each direction.
   x and y direction represent the magnetic field on earth in
   absence of any other strong magnetic fields.
*/
float getDirection(float x, float y)
{
  // Calculate the angle of the vector y,x
  float heading = (atan2(y, x) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }

  Serial.print("Compass Heading: ");
  Serial.println(heading);

  return heading;
}

