/************************************************
 *
 * Clock calendar
 * Displays time, day and date, temperature'
 * humidity, pressure from BME280 and current
 * GBP->TWD exchange rate.
 *
 * Uses Nextion 4.3in display programmed with BME280.tft
 *
 *
 * Receives both currency info and calendar info in
 * JSON format.
 *
 * Uses Google CalendarGet2 script and fixwe.io for
 * currency. Currency is oly read once a day as
 * number of reads is limited to 1000/mth for the free
 * service
 *
 * Uses NTP server to supply time every 5 mins which is
 * stored and updated locally in case of read failure
 *
 * V1.0
 *
 */
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include "TimeLib.h"
#include "BME280I2C.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiUdp.h>
#include "DebugMacros.h"
#include <Thread.h>
#include <ThreadController.h>

#include "common.h"
#include "getCalendar.h"
#include "getCurrency.h"
#include "getBME280.h"
#include "getNtpTime.h"
#include "parseJson.h"
#include "parseString.h"
#include "updateClock.h"


WiFiUDP Udp;
String rstring;
String currency;
time_t prevDisplay=0;
float * readings;

ThreadController ThreadControl = ThreadController();
Thread* gcalThread    = new Thread();
Thread* currThread    = new Thread();
Thread* ntpThread     = new Thread();
Thread* BME280Thread   = new Thread();

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

IPAddress ip(192,168,1,145);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void handleNTP(void)
{
  if (timeStatus()!=timeSet)
  {
      setSyncProvider(getNtpTime);
      setSyncInterval(300);
  }
if (timeStatus()!=timeNotSet)
{
    if (now() != prevDisplay)
    {
        prevDisplay=now();

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
}
}
}

void handleGCAL(void)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    rstring = getCalendar();
    // send parsed string to Nextion
    nexSerial.print("t12.txt=");
    nexSerial.print(parse_response(rstring));
    nexSerial.write(0xff);
    nexSerial.write(0xff);
    nexSerial.write(0xff);

  }
}

void handleCurr(void)
{
  String payload = getCurrency();
  DPRINT("Curr:"); DPRINTLN(payload);
  if (payload != "") {
    double conversion =  parse_json(payload);
    currency = String(F("1 GBP = ")) + conversion + (F(" NT$"));
  }
  else
    currency = "Could not get currency";
    nexSerial.print("t13.txt=");
    nexSerial.print(currency);
    nexSerial.write(0xff);
    nexSerial.write(0xff);
    nexSerial.write(0xff);
}

void handleBME280 (void)
{
  reading = getBME280();

  nexSerial.print("t0.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  nexSerial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  nexSerial.print(reading[0]);  // This is the value you want to send to that object and atribute mentioned before.
  nexSerial.print("degC");
  nexSerial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  nexSerial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  nexSerial.write(0xff);
  nexSerial.write(0xff);

  nexSerial.print("t1.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  nexSerial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  nexSerial.print(reading[1]);  // This is the value you want to send to that object and atribute mentioned before.
  nexSerial.print("%");
  nexSerial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  nexSerial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  nexSerial.write(0xff);
  nexSerial.write(0xff);

  nexSerial.print("t2.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  nexSerial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  nexSerial.print(reading[2]);  // This is the value you want to send to that object and atribute mentioned before.
  nexSerial.println("Bar");
  nexSerial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  nexSerial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  nexSerial.write(0xff);
  nexSerial.write(0xff);

}

void setup() {
  // put your setup code here, to run once:
  nexSerial.begin(9600);
  Serial.begin(115200);
  Wire.begin();
  // init the BME280
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_bar);

  WiFi.begin(ssid, pass);
  WiFi.config(ip,gateway,subnet);
  while (WiFi.status() != WL_CONNECTED ) {
    yield();
    delay(500);
    Serial.print(".");
  }
  MDNS.begin(uhost);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", uhost);

  Udp.begin(localPort); // for NTP

  // initiate threads
  gcalThread->onRun(handleGCAL);    // Update Google Calendar data.
  gcalThread->setInterval(GCAL_TIME);  // 30 minute
  ThreadControl.add(gcalThread);
  currThread->onRun(handleCurr);    // Update currency data
  currThread->setInterval(CURR_TIME);   // 60 minute
  ThreadControl.add(currThread);
  ntpThread->onRun(handleNTP);    // Update Time
  ntpThread->setInterval(NTP_TIME);   // 5 minute
  ThreadControl.add(ntpThread);
  bme280Thread->onRun(handleBME280);    // temperature and humidity
  bme280Thread->setInterval(BME280_TIME);   // 10 minutes
  ThreadControl.add(bme280Thread);


  // first time do straightaway
  handleGCAL();
  handleCurr();
  handleNTP();
  handleBME280();
}

void loop() {
  // put your main code here, to run repeatedly:
  ThreadControl.run();
  httpServer.handleClient();
}
