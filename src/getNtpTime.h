#ifndef _GNTP_H
#define _GNTP_H
extern WiFiUDP Udp;
time_t getNtpTime();
void sendNTPpacket(IPAddress &);
#endif
