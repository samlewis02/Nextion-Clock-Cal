#ifndef _UPCLK_H
#define _UPCLK_H
extern String currency;
extern String rstring;
extern String strth;
extern String parse_response(String);
extern GxGDEW042T2 display;
extern void show7seg (uint16_t, uint16_t, byte, uint16_t);
extern void showColon (uint16_t, uint16_t, uint16_t);
void clockDisplay();
void initDisplay();
String daySuffix (uint8_t);
#endif
