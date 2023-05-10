# NanoChron
## Tiny but stupid accurate clock

I'll add files to this repo in the coming weeks so look out for that :)

This project has been one I've been procrastinating on for a bit but I think I'll be able to finish it soon. NanoChron came about when I decided to make a replacement clock after an analog clock in my bathroom broke, which turned into a project encompassing 2 of my interests: Code and 3D Design/Printing.

The beauty of this clock is that all the sensors and display are run purely through I2C, there are no other connections, and it only requires minimal soldering. This is achieved by implementing Adafruit's STEMMA QT connectors and essentially just daisy-chaining all the components through the interface 😆.

The Setup
---

**Parts**
+ [Adafruit QTPY ESP32-C3](https://www.adafruit.com/product/5405 "Store Link") (realistically you could use any board that runs Arduino, has a Stemma QT connector, and WiFi compatibility)
+ [LTR-303 Light Sensor](https://www.adafruit.com/product/5610), this is so we can adaptively dim the clock face because at max brightness the display is ***BRIGHT***
+ [DS3231 RTC Module](https://www.adafruit.com/product/5188), like I said, this thing is gonna be stupid accurate
+ [CR1220 Coin Cell](https://www.adafruit.com/product/380), to power the RTC (you could probably get these cheaper off of amazon but adafruit had a direct link and I was lazy)
+ [Seven Segment Display + I2C Interface](https://www.adafruit.com/product/1002), I used the white 7 segment, however any color works :)
+ [Stemma QT Connectors](https://www.adafruit.com/product/4399)
