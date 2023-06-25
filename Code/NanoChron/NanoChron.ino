/******************************************
  NanoChron

  Written by Adam E. (Moonrise_Sunset)

  Big thanks to Adafruit for making it so easy to use all these sensors :)

  The only parts of the code that need modification to work are in the config.h file.

  This code is protected under GNU GPLv3
 ******************************************/

// Display libraries
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// Wi-Fi libraries
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

// Time and light sensor libraries
#include <NTPClient.h>
#include "RTClib.h"
#include "Adafruit_LTR329_LTR303.h"

// Setting up all the libraries
Adafruit_7segment display = Adafruit_7segment();
Adafruit_LTR303 light = Adafruit_LTR303();

// NTP and time variables
WiFiUDP ntpUDP;
NTPClient Time(ntpUDP, "north-america.pool.ntp.org", (TIMEZONE * 3600));
RTC_DS3231 rtc;

/* Set the screen brightness based on the time */
void setBright( void ) {
    uint16_t total, infra;

    light.readBothChannels(total, infra); // Read the sensor data to total and infra
    uint8_t visible = abs(total - infra);

    uint8_t vmax = max(vmax, visible); // Update the max brightness to the range of the sensor

    display.setBrightness((uint8_t)(visible / (float)vmax * 15.0 + 0.5)); // Map the sensor brightness to an int 0-15
    display.writeDisplay(); // Update the display
}

/* Check if we are in daylight savings, adapted from https://stackoverflow.com/a/5590518/15962903 */
bool isDST( uint8_t day, uint8_t month, uint8_t dow ){
    if (month < 3 || month > 11) return false; // January, February, and December are out.
    if (month > 3 && month < 11) return true; // April to October are in
    int previousSunday = day - dow;
    if (month == 3) return previousSunday >= 8; // In March, we are DST if our previous Sunday was on or after the 8th.
    return previousSunday <= 0; // In November we must be before the first Sunday to be DST. That means the previous Sunday must be before the 1st.
}

/* Update the time from the NTP server */
void TimeRef( void ) {
    Time.update();
    DateTime currentRTCTime = rtc.now();

    uint8_t sref = Time.getSeconds();
    uint8_t mref = Time.getMinutes();
    uint8_t href = Time.getHours();

    uint8_t day = currentRTCTime.day();
    uint8_t month = currentRTCTime.month();
    uint8_t dow = currentRTCTime.dayOfTheWeek();

    if(ENABLE_DST && isDST(day, month, dow)) href += 1;

    rtc.adjust(DateTime(currentRTCTime.year(), month, day, href, mref, sref + 8));
}

/* Draw the current time to the screen */
void GetTime( void ) {
    uint8_t s, m, h;

    DateTime currentRTCTime = rtc.now();
    s = currentRTCTime.second();
    m = currentRTCTime.minute();
    h = currentRTCTime.hour();

    if (h == 12 and m == 30) TimeRef();

    if (HOURS_12) {
        if (h > 12) h = h - 12;
        if (h == 0) h = 12;
    }

    display.clear(); // Clear the display

    if (h >= 10) display.writeDigitNum(0, h / 10);
    display.writeDigitNum(1, (h % 10));
    display.drawColon(1);
    display.writeDigitNum(3, (m / 10));
    display.writeDigitNum(4, (m % 10));

    display.writeDisplay(); // Update the display

    if (DYNAMIC_BRIGHTNESS) setBright();
}

/* Code that runs when the device starts up */
void setup( void ) {
    // Serial.begin(115200); // for debug

    // Start the DS3231 onboard RTC and set the time to when the sketch was uploaded
    rtc.begin();

    // Start the light sensor
    light.begin();
    light.setGain(LTR3XX_GAIN_96);
    light.setIntegrationTime(LTR3XX_INTEGTIME_100);
    light.setMeasurementRate(LTR3XX_MEASRATE_200);

    // Start the 7 seg display
    display.begin(0x70);

    // If we have DYNAMIC_BRIGHTNESS disabled, set the brightness
    if (!DYNAMIC_BRIGHTNESS) display.setBrightness(BRIGHTNESS);

    //Starting the WiFi and NTP
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        //Serial.print("-");
        delay(500);
    }

    //Do an intial sync from the NTP to the RTC
    TimeRef();
}

/* Main program loop */
void loop( void ) {
    GetTime();
    delay(100); // Sleep .1 seconds to reduce load
}
