/****************************************************************************************************************************
  ESP32_FS_EthernetWebServer.ino - Dead simple FS Ethernet WebServer for ES32 using Ethernet shields

  For Ethernet shields using WT32_ETH01 (ESP32 + LAN8720)

  WebServer_WT32_ETH01 is a library for the Ethernet LAN8720 in WT32_ETH01 to run WebServer

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_WT32_ETH01
  Licensed under MIT license
 *****************************************************************************************************************************/
/*****************************************************************************************************************************
   The Arduino board communicates with the shield using the SPI bus. This is on digital pins 11, 12, and 13 on the Uno
   and pins 50, 51, and 52 on the Mega. On both boards, pin 10 is used as SS. On the Mega, the hardware SS pin, 53,
   is not used to select the Ethernet controller chip, but it must be kept as an output or the SPI interface won't work.
*****************************************************************************************************************************/
/*****************************************************************************************************************************
   How To Use:
   1) Upload the contents of the data folder with MkSPIFFS Tool ("ESP8266 Sketch Data Upload" in Tools menu in Arduino IDE)
   2) or you can upload the contents of a folder if you CD in that folder and run the following command:
      for file in `\ls -A1`; do curl -F "file=@$PWD/$file" localIPAddress/edit; done
   3) access the sample web page at http://localIPAddress/
*****************************************************************************************************************************/

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3

#define USE_LITTLEFS                false
#define USE_SPIFFS                  true

// For WT32_ETH01 only (A0 = IO36 = 36)
#if defined(ARDUINO_WT32_ETH01)
  #define A0        36
#endif

// For ESP32
#if USE_LITTLEFS
  //LittleFS has higher priority
  #include "FS.h"

  // Check cores/esp32/esp_arduino_version.h and cores/esp32/core_version.h
  //#if ( ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(2, 0, 0) )  //(ESP_ARDUINO_VERSION_MAJOR >= 2)
  #if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 2) )
    #warning Using ESP32 Core 1.0.6 or 2.0.0+
    // The library has been merged into esp32 core from release 1.0.6
    #include <LittleFS.h>

    FS* filesystem =            &LittleFS;
    #define FileFS              LittleFS
    #define CurrentFileFS       "LittleFS"
  #else
    #warning Using ESP32 Core 1.0.5-. You must install LITTLEFS library
    // The library has been merged into esp32 core from release 1.0.6
    #include <LITTLEFS.h>             // https://github.com/lorol/LITTLEFS

    FS* filesystem =            &LITTLEFS;
    #define FileFS              LITTLEFS
    #define CurrentFileFS       "LittleFS"
  #endif

  #ifdef USE_SPIFFS
    #undef USE_SPIFFS
  #endif

  #define USE_SPIFFS                  false
#elif USE_SPIFFS
  #include "FS.h"
  #include <SPIFFS.h>

  FS* filesystem =          &SPIFFS;
  #define FileFS            SPIFFS
  #define CurrentFileFS     "SPIFFS"
#endif

#include <WebServer_WT32_ETH01.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>

WebServer server(80);
String dbMeterIp = "192.168.0.20";

// Select the IP address according to your local network
IPAddress myIP(192, 168, 2, 232);
IPAddress myGW(192, 168, 2, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

void storeIp (String ip);

//format bytes
String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename)
{
  if (server.hasArg("download"))
  {
    return "application/octet-stream";
  }
  else if (filename.endsWith(".htm"))
  {
    return "text/html";
  }
  else if (filename.endsWith(".html"))
  {
    return "text/html";
  }
  else if (filename.endsWith(".css"))
  {
    return "text/css";
  }
  else if (filename.endsWith(".js"))
  {
    return "application/javascript";
  }
  else if (filename.endsWith(".png"))
  {
    return "image/png";
  }
  else if (filename.endsWith(".gif"))
  {
    return "image/gif";
  }
  else if (filename.endsWith(".jpg"))
  {
    return "image/jpeg";
  }
  else if (filename.endsWith(".ico"))
  {
    return "image/x-icon";
  }
  else if (filename.endsWith(".xml"))
  {
    return "text/xml";
  }
  else if (filename.endsWith(".pdf"))
  {
    return "application/x-pdf";
  }
  else if (filename.endsWith(".zip"))
  {
    return "application/x-zip";
  }
  else if (filename.endsWith(".gz"))
  {
    return "application/x-gzip";
  }

  return "text/plain";
}

bool handleFileRead(String path)
{

  if (path.endsWith("/"))
  {
    path += "index.htm";
  }

  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";

  if (filesystem->exists(pathWithGz) || filesystem->exists(path))
  {
    if (filesystem->exists(pathWithGz))
    {
      path += ".gz";
    }

    File file = filesystem->open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }

  return false;
}

void initFS()
{
  // Initialize LittleFS/SPIFFS file-system
  // Format SPIFFS if not yet
  if (!FileFS.begin(true))
  {

    if (!FileFS.begin())
    {
      while (true)
      {
#if USE_LITTLEFS
        Serial.println(F("LittleFS failed!. Please use SPIFFS."));
#else
        Serial.println(F("SPIFFS failed!. Please use LittleFS."));
#endif
        // Stay forever here as useless to go further
        delay(5000);
      }
    }
  }
}

HTTPClient http;

void handleApiData ()
{
  server.enableCrossOrigin();
  if (!WT32_ETH01_isConnected())
  {
    server.send(500, "text/plain", "no Network");
    return;
  }
  

  http.begin("http://"+dbMeterIp+"/xml/Global.xml");
  http.setConnectTimeout(200);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      server.send(200, "text/xml", payload);
      http.end();
      return;
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  server.send(500, "text/plain", "unable to reach dbMeter");
  http.end();
}

void handleApiTargetIp ()
{
  server.enableCrossOrigin();
  if (server.method() == HTTP_POST)
  {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, server.arg(0));
    dbMeterIp = doc["ip"].as<String>();
    storeIp(dbMeterIp);
    server.send(200);
  }

  if (server.method() == HTTP_GET)
  {
    server.send(200, " application/json", "{\"ip\": \""+ dbMeterIp +"\"}");
  }

  server.send(400, "method not found");
}

void handleApiNetConf ()
{
  server.enableCrossOrigin();
  if (server.method() == HTTP_POST)
  {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, server.arg(0));
    Serial.println(error.f_str());
    dbMeterIp = doc["ip"].as<String>();
    server.send(200);
  }

  if (server.method() == HTTP_GET)
  {
    server.send(200, " application/json", "{\"ip\": \""+ dbMeterIp +"\"}");
  }

  server.send(400, "method not found");
}

void initWebserver()
{
  //SERVER INIT 
  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]()
  {
    if (!handleFileRead(server.uri()))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  // Web Page handlers
  server.serveStatic("/", FileFS, "/index.html");

  // api
  server.on("/api/data", handleApiData);
  server.on("/api/targetIp", handleApiTargetIp);

  server.begin();
}

void ipFromStore ()
{
  File file = SPIFFS.open("/ip.tx");

  dbMeterIp = file.readString();
  file.close();
}

void storeIp (String ip)
{
  File file = SPIFFS.open("/ip.tx", FILE_WRITE);
  file.print(dbMeterIp);
  file.close();
}

void setup()
{
  Serial.begin(115200);

  // Using this if Serial debugging is not necessary or not using Serial port
  while (!Serial && (millis() < 3000));

  Serial.print("\nStarting webserver on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);

  // To be called before ETH.begin()
  WT32_ETH01_onEvent();

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO,
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  //ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_waitForConnect();

  MDNS.begin("dbinator");
  initFS();
  ipFromStore();
  initWebserver();

  Serial.print("HTTP server started @");
  Serial.println(ETH.localIP());

  Serial.print(F("Open http://"));
  Serial.print(ETH.localIP());
  Serial.println(F("/ to see Db Meter"));
}

void loop()
{
  server.handleClient();
}