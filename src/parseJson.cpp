//
// Parse a JSON string from getCurrency passed as parameter
// Extract EUR->GBP and EUR->TWD
// Calculate GBP->TWD
//
// Returns: double with conversion value
//
#include "Arduino.h"
#include <ArduinoJson.h>
#include "parseJson.h"
#include "DebugMacros.h"

double parse_json(String r)
{
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);

  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(r);
  if (!root.success()) {
    DPRINTLN(F("Parsing failed!"));
    return 0;
  }
  else {
    double GBPrate = root["rates"]["GBP"];
    double TWDrate = root["rates"]["TWD"];
    return TWDrate / GBPrate;
   }
}
