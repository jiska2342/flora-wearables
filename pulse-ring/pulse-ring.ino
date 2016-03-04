
/*  Pulse Sensor Amped 1.4 modified for FLORA and neo pixels
(originally by Joel Murphy and Yury Gitman http://www.pulsesensor.com)

----------------------  Notes ----------------------  ----------------------
This code:
1) Blinks the FLORA red onboard LED to User's Live Heartbeat
2) Fades an NeoPixel ring according to the pulse shape
3) Determines BPM
4) Prints All of the Above to Serial

*/

#include <Adafruit_NeoPixel.h>

//  Variables
#define PULSE_PIN     10  // Pulse Sensor purple wire connected to analog pin 0
#define LED_PIN        7  // pin to blink onboard LED at each beat
#define BRIGHTNESS    40  //initial brightness
#define PIXEL_PIN      6  //pin to which he neopixel ring is connected, set to 8 for onboard neopixel
#define PIXEL_COUNT   12  //ring size, set to 1 for onboard neopixel
#define PIXEL_RATE    15  //display every n-th sensor reading, typical values: 10-100
                          //the lower, the more of the pulse shape is visible, but is fastly overridden by new values

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
volatile int currentPixel = 1;     //remember at which pixel we were for animation effect
volatile int timePixel = 0;        //global variable to count timer

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse

//neo pixel output
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(LED_PIN, OUTPUT);        // pin that will blink to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
  strip.begin();                    // initialize pixels
  strip.setBrightness(BRIGHTNESS);
  // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
  // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //   analogReference(EXTERNAL);
}


//  Where the Magic Happens
void loop() {

  serialOutput() ;

  if (QS == true) {    // A Heartbeat Was Found
    // BPM and IBI have been Determined
    // Quantified Self "QS" true when arduino finds a heartbeat
    serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
    QS = false;                      // reset the Quantified Self flag for next time
  }

  delay(5);                       // take a break...

}


/*
  Set the neo pixel ring clockwise.
  Color change: low red (lowest pressure) -> medium blue -> medium cyan -> bright green -> bright orange -> bright red (highest pressure)
  This routine is called by the 2ms reading timer.
*/
void neoBeat() {

  //wait until total time is reached
  timePixel++;

  if (timePixel > PIXEL_RATE) {

    //signal values between 0 and 1024, map them to led range between 0 and 255
    int ledReading = map(Signal, 0, 1024, 0, 255);

    uint32_t color;
    float brightness = ledReading / 255.0;

    if (ledReading < 85) {
      color = strip.Color((255 - ledReading * 3) * brightness, 0, ledReading * 3 * brightness);
    }
    else if (ledReading < 170) {
      ledReading -= 85;
      color = strip.Color(0, ledReading * 3 * brightness, (255 - ledReading * 3) * brightness);
    }
    else {
      ledReading -= 170;
      color = strip.Color(ledReading * 3 * brightness, (255 - ledReading * 3) * brightness, 0);
    }

    strip.setPixelColor(currentPixel, color);
    strip.show(); //send updates to pixel ring

    //start timer again
    timePixel = 0;

    //take next pixel
    currentPixel++;
    if (currentPixel > PIXEL_COUNT) {
      currentPixel = 0;
    }
  }

}




