# NanoChron



# NOTE:

**I'm gonna archive NanoChron most likely for an indefinite period, I may come back to it sometime in the future, but not for now.**

---
## Tiny but stupid accurate clock

This project has been one I've been procrastinating on for a bit but I think I'll be able to finish it soon. NanoChron came about when I decided to make a replacement clock after an analog clock in my bathroom broke, which turned into a project encompassing 2 of my interests: Code and 3D Design/Printing.

The beauty of this clock is that all the sensors and display are run purely through I2C, there are no other connections, and it only requires minimal soldering. This is achieved by implementing Adafruit's STEMMA QT connectors and essentially just daisy-chaining all the components through the interface 😆.

**Note: the 3d printed housing is still WIP, it should be done soon**

The Setup
---

**Parts**
+ [Adafruit QTPY ESP32-C3](https://www.adafruit.com/product/5405 "Store Link") (realistically you could use any board that runs Arduino, has a Stemma QT connector, and WiFi compatibility)
+ [LTR-303 Light Sensor](https://www.adafruit.com/product/5610), this is so we can adaptively dim the clock face because at max brightness the display is ***BRIGHT***
+ [DS3231 RTC Module](https://www.adafruit.com/product/5188), like I said, this thing is gonna be stupid accurate
+ [CR1220 Coin Cell](https://www.adafruit.com/product/380), to power the RTC (you could probably get these cheaper off of amazon but adafruit had a direct link and I was lazy)
+ [Seven Segment Display + I2C Interface](https://www.adafruit.com/product/1002), I used the white 7 segment, however any color works :)
+ [Stemma QT Connectors](https://www.adafruit.com/product/4399)

---
**Note that this project does require a small amount of soldering and 3d printing**

[Here](https://learn.adafruit.com/adafruit-led-backpack/0-dot-56-seven-segment-backpack-assembly "Instructions") are the instructions on how to solder the 7 segment display, **DON'T solder the 4 pin header, only solder the 7 seg display to the backpack board,** we don't need it and it may prevent the board from fitting into the 3dp board housing (coming soon lol).

The Code
---

I designed (is that the right word idk) the code to be as easy to work with as possible, with all the timezone/time, brightness, and WiFi settings located on the **config.h** file, none of the main arduino code needs to be modified 😄.
