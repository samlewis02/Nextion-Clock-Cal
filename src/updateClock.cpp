// Update the Waveshare 4.2in display
//
// Assumes setIime() holds current time
// Global data as inputs:
// String rstring holds calendar info
// String strth holds temperature/humidity info
// double currency holds GBP to TWD conversion value
//
#include "Arduino.h"
// FreeFonts from Adafruit_GFX
#include "Timelib.h"
#include "common.h"
#include "updateClock.h"

void clockDisplay()
{
// hour < 10 don't print digit in first location
if (hour() > 9)
{
    nexSerial.print("cov ");
    nexSerial.print(hour());
    nexSerial.print(",t9.txt,0);
    nexSerial.write(0xff);
    nexSerial.write(0xff);
    nexSerial.write(0xff);
}
else
{
    nexSerial.print("cov ");
    nexSerial.print(hour());
    nexSerial.print(",t9.txt,2);
    nexSerial.write(0xff);
    nexSerial.write(0xff);
    nexSerial.write(0xff);

}
nexSerial.print("cov ");
nexSerial.print(minute());
nexSerial.print(",t10.txt,2);
nexSerial.write(0xff);
nexSerial.write(0xff);
nexSerial.write(0xff);

nexSerial.print("cov ");
nexSerial.print(second());
nexSerial.print(",t11.txt,2);
nexSerial.write(0xff);
nexSerial.write(0xff);
nexSerial.write(0xff);

// *** Note: daySuffix is edit to DateStrings and TimeLib.h in Timelib library
String daynow = String(dayStr(weekday())) + " " + day() + daySuffix(day()) + " " + monthStr(month());
nexSerial.print("t8.txt=");
nexSerial.print(daynow);
nexSerial.write(0xff);
nexSerial.write(0xff);
nexSerial.write(0xff);


// Show calendar info
nexSerial.print("t12.txt=");
nexSerial.print(parse_response(rstring));
nexSerial.write(0xff);
nexSerial.write(0xff);
nexSerial.write(0xff);

// Show temp and humidity

display.setTextColor(GxEPD_WHITE);
display.setFont(&FreeMonoBold9pt7b);
display.setCursor(75, 30);
display.println(strth);

// show currency
nexSerial.print("t13.txt=");
nexSerial.print(currency);
nexSerial.write(0xff);
nexSerial.write(0xff);
nexSerial.write(0xff);

}

String daySuffix (uint8_t day)
{
String ordinal_suffix;
switch (day) {
  case 1:
  case 21:
  case 31: ordinal_suffix = "st"; break;
  case 2:
  case 22: ordinal_suffix = "nd"; break;
  case 3:
  case 23: ordinal_suffix = "rd"; break;
  default: ordinal_suffix = "th";
};
return ordinal_suffix;
}
