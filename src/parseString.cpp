/*
  // Parse a JSON string passed as parameter
  // Extract EUR->GBP and EUR->TWD
  // Calculate GBP->TWD
  //
  // Returns: double with conversion value
  //
  JSON format:

  {
  "success":"OK",
  "eventsToday":[
  {"title":"Dentist Chi","time":" 11:00"},
  {"title":"Forecast (27° | 24°)","time":" "}
  etc
  ],
  "eventsTomro":[
  {"title":"No events today","time":" "},
  {"title":"Forecast (28° | 24°)","time":" "}
  etc
  ]
  }
*/
#include "Arduino.h"
#include "Timelib.h"
#include <ArduinoJson.h>
#include "parseString.h"
#include "DebugMacros.h"

String parse_response(String r)
{
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);
  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(r);
  String todayStr = "   Today\n";
  if (root["success"] == "OK") {
    //DRPINTLN("Today");
    for (int i = 0; i < 10; i++)
    {
      //DRPINTLN(i);
      String ttitle = root["eventsToday"][i]["title"];
      if (ttitle == "")
        break;
      String ttime = root["eventsToday"][i]["time"];
      String tevent = String("      ") + ttitle + " " + ttime + '\n';
      todayStr += tevent;
    }
    DPRINTLN(todayStr);

    //DRPINTLN("Tomorrow");
    String tomorrowStr = "   Tomorrow\n";
    for (int j = 0; j < 10; j++)
    {
      String mtitle = root["eventsTomro"][j]["title"];
      if (mtitle == "")
        break;
      String mtime = root["eventsTomro"][j]["time"];
      String mevent = String("      ") + mtitle + " " + mtime + '\n';
      tomorrowStr += mevent;
    }
    DPRINTLN(tomorrowStr);
    if (minute() % 2) return todayStr;
    else return tomorrowStr;
  }
  else if (root["success"] == "NOK") return root["error"];
  else return "unknown error";
}
