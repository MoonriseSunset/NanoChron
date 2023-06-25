/*
  NanoChron

  Written by Adam E. (Moonrise_Sunset)

  Big thanks to Adafruit for making it so easy to use all these sensors :)

  The only parts of the code that need modification to work are in the config.h file.

  This code is protected under GNU GPLv3
*/

//Display Libraries
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//WiFi Libraries
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

//Time and Light Sensor Libraries
#include <NTPClient.h>
#include "RTClib.h"
#include "Adafruit_LTR329_LTR303.h"

//Setting up all the libraries
Adafruit_7segment display = Adafruit_7segment();

RTC_DS3231 rtc;

Adafruit_LTR303 light = Adafruit_LTR303();

WiFiUDP ntpUDP;
NTPClient Time(ntpUDP, "north-america.pool.ntp.org", (Timezone * 3600));

//Light Sensor variables
int visible;
uint16_t total, infra;
int vmax = 10;
int brange[] = {0, vmax / 15, (2 * vmax) / 15, vmax / 5, (4 * vmax) / 15, vmax / 3, (2 * vmax) / 5, (7 * vmax) / 15, (8 * vmax) / 15, (3 * vmax) / 5, (2 * vmax) / 3, (11 * vmax) / 15, (4 * vmax) / 5, (13 * vmax) / 15, (14 * vmax) / 15, vmax};
unsigned int bcounter;

//RTC variables
unsigned int s, m, h, day, month, dow;

//NTP Reference variables
unsigned int sref, mref, href;

//display/test variables
unsigned int counter = 0;

void adjustBscale() {
  while (bcounter <= 15) {
    brange[bcounter] = (bcounter * vmax) / 15;
    //Serial.print(brange[bcounter]);
    //Serial.print(" ,");
    bcounter++;
  }
  //Serial.println(" ");
  bcounter = 0;
}

void setBright() {

  light.readBothChannels(total, infra);
  visible = abs(total - infra);

  if (visible > vmax) {
    vmax = visible;
    adjustBscale();
    //Serial.println(vmax);
  }

  if (visible <= brange[0]) {
    display.setBrightness(0);
  }
  else if (visible > brange[0] and visible <= brange[1]) {
    display.setBrightness(1);
  }
  else if (visible > brange[1] and visible <= brange[2]) {
    display.setBrightness(2);
  }
  else if (visible > brange[2] and visible <= brange[3]) {
    display.setBrightness(3);
  }
  else if (visible > brange[3] and visible <= brange[4]) {
    display.setBrightness(4);
  }
  else if (visible > brange[4] and visible <= brange[5]) {
    display.setBrightness(5);
  }
  else if (visible > brange[5] and visible <= brange[6]) {
    display.setBrightness(6);
  }
  else if (visible > brange[6] and visible <= brange[7]) {
    display.setBrightness(7);
  }
  else if (visible > brange[7] and visible <= brange[8]) {
    display.setBrightness(8);
  }
  else if (visible > brange[8] and visible <= brange[9]) {
    display.setBrightness(9);
  }
  else if (visible > brange[9] and visible <= brange[10]) {
    display.setBrightness(10);
  }
  else if (visible > brange[10] and visible <= brange[11]) {
    display.setBrightness(11);
  }
  else if (visible > brange[11] and visible <= brange[12]) {
    display.setBrightness(12);
  }
  else if (visible > brange[12] and visible <= brange[13]) {
    display.setBrightness(13);
  }
  else if (visible > brange[13] and visible <= brange[14]) {
    display.setBrightness(14);
  }
  else if (visible > brange[14] and visible <= brange[15]) {
    display.setBrightness(15);
  }
  
  display.writeDisplay();
}

//this module of code was taken from https://stackoverflow.com/questions/5590429/calculating-daylight-saving-time-from-only-date (and modified a little so it works here)
bool IsDST(int day, int month, int dow){
    //January, february, and december are out.
    if (month < 3 || month > 11) { return false; }
    //April to October are in
    if (month > 3 && month < 11) { return true; }
    int previousSunday = day - dow;
    //In march, we are DST if our previous sunday was on or after the 8th.
    if (month == 3) { return previousSunday >= 8; }
    //In november we must be before the first sunday to be dst.
    //That means the previous sunday must be before the 1st.
    return previousSunday <= 0;
}

void TimeRef() {
  Time.update();
  DateTime hardware = rtc.now();
  sref = Time.getSeconds();
  mref = Time.getMinutes();
  href = Time.getHours();
  if(DSTEnable == true){
    DateTime hardware = rtc.now();
    day = hardware.day();
    month = hardware.month();
    dow = hardware.dayOfTheWeek();
    if(IsDST(day, month, dow) == true){
      href += 1;
    }
  }

  rtc.adjust(DateTime(hardware.year(), hardware.month(), hardware.day(), href, mref, sref + 8));
}

void GetTime() {
  DateTime hardware = rtc.now();
  s = hardware.second();
  m = hardware.minute();
  h = hardware.hour();
  if (h > 13 and hourmode == true) {
    h = h - 12;
  }
  display.clear();
  if (h >= 10) {
    display.writeDigitNum(0, h / 10);
    display.writeDigitNum(1, (h % 10));
    display.drawColon(1);
    display.writeDigitNum(3, (m / 10));
    display.writeDigitNum(4, (m % 10));
  }
  else {
    display.writeDigitNum(1, (h));
    display.drawColon(1);
    display.writeDigitNum(3, (m / 10));
    display.writeDigitNum(4, (m % 10));
  }
  display.writeDisplay();

  if (DynamicBrightness == true) {
    setBright();
  }
}

void setup() {
  //Serial.begin(115200); //for debug

  //Starting the DS3231 onboard RTC and setting the time to when the sketch was uploaded
  rtc.begin();

  //Starting the light sensor
  light.begin();
  light.setGain(LTR3XX_GAIN_96);
  light.setIntegrationTime(LTR3XX_INTEGTIME_100);
  light.setMeasurementRate(LTR3XX_MEASRATE_200);

  //Starting the 7 seg display
  display.begin(0x70);
  //If we have DynamicBrightness disabled, set the brightness
  if(DynamicBrightness == false){
    display.setBrightness(Brightness);
  }

  //Starting the WiFi and NTP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print("-");
    delay(500);
  }
  //Doing an intial sync from the NTP to the RTC
  TimeRef();
}

void loop() {
  if (h == 12 and m == 30) {
    TimeRef();
  }
  GetTime();

}
