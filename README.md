# Flora Wearables
Arduino wearables that were tested with the FLORA platform.

## compass-ring
This code is taking inputs from the compass sensor which are then displayed on a NeoPixel ring. 
Directions are mapped to colors: red is south, cyan is north.
The current direction is displayed using the brightness of the LEDs. Hence, even angular changes below the amount of angles from the LED positions can be displayed.

![alt tag](https://pbs.twimg.com/media/CcpBYnPW4AAS50b.jpg:large)

## pulse-ring
This code is for the pulse sensor from pulsesensor.com. All interrupt settings were changed to be compatible to FLORA. Detected heartbeats are shown by the red flashing onboard LED. The actual pulse curve can be shown on the onboard NeoPixel (color and brightness change) or on an external NeoPixel ring (animation on the whole ring to show the pulse history).

![alt tag](https://pbs.twimg.com/media/CctYpz0WAAAobSk.jpg:large)
