//
// Get exchange rate from fixer.io website
// Get EUR->GBP and EUR->TWD
// Calculate GBP->TWD
//
// Returns: JSON string which can be parsed
//
#include "Arduino.h"
#include "common.h"
#include <ESP8266HTTPClient.h>
#include "getCurrency.h"
#include "DebugMacros.h"

void getCurrency() {
  String response;
  HTTPClient http;
  DPRINT("[HTTP] begin...\n");
  // configure traged server and url
  http.begin(CURRURL); //HTTP

  DPRINT("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      response = http.getString();
      DPRINTLN(response);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    response = "";
  }

  http.end();
  return response;
}
