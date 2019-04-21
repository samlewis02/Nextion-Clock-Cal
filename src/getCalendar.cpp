#include <WiFiClientSecure.h>
#include "getCalendar.h"
#include "common.h"
#include "DebugMacros.h"
WiFiClientSecure client;

String WebFetch(String url)
{
  String Response;
  bool Redirect = 0;

  if (!client.connect(host, httpsPort))
    DPRINTLN("GCAL:No connection");
  else
  {
    DPRINTLN("GCAL:Connect");
        DPRINTLN("GET " + url + " HTTP/1.0\nHost: " + host + '\n' + "Connection: close\n");

    // Make a HTTP request:
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.0");
    client.print("Host: ");
    client.println(host);
    client.println("Connection: close");
    client.println();
    String header;
    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      header = header + line + "\n";
      if (line.startsWith("Location: "))
      {
        Redirect = 1;
        Response = line.substring(line.indexOf("http"));
        DPRINT("GCAL:REDIRECT:"); DPRINTLN(Response);
      }
      if (line == "\r")
        break;
    }
    DPRINTLN("GCAL:HEADER:"); DPRINTLN(header);

    String body;
    int linec = 0;
    while (client.available())
    {
      String line = client.readStringUntil('\n');
      body = body + line + "\n";
      linec++;
      if (line == "\r")
      {
        DPRINT("End of body reached. line = ");DPRINTLN(linec);

        break;
      }
    }

    if (!Redirect)
    {
      //DPRINTLN("GCAL:BODY:"); DPRINTLN(body);
      Response = body;
    }

    client.stop();
  }

  return(Response);
}


String getCalendar()
{
  String Return1;
  String Return2;
  Return1 = WebFetch(GCALURL);
  Return2 = WebFetch(Return1);

  return(Return2);

}
