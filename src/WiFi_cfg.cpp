/**
 * @file WiFi_cfg.cpp
 * @author Jordi Gauchía (jgauchia@jgauchia.com)
 * @brief  ESP8266 Captive Portal Class
 * @version 0.1
 * @date 2023-01-29
 */
#include "WiFi_cfg.h"

/**
 * @brief WiFi Configuration Files
 *
 */
String WiFi_cfg::ssid = "";
String WiFi_cfg::pass = "";
String WiFi_cfg::ip = "";
String WiFi_cfg::gateway = "";
const char *WiFi_cfg::ssidPath = "/ssid.txt";
const char *WiFi_cfg::passPath = "/pass.txt";
const char *WiFi_cfg::ipPath = "/ip.txt";
const char *WiFi_cfg::gatewayPath = "/gateway.txt";

/**
 * @brief Construct a new WiFi_cfg::WiFi_cfg object
 *
 */
WiFi_cfg::WiFi_cfg()
{
  server = new AsyncWebServer(80);
}

/**
 * @brief Init WiFi
 *
 * @return true -> WiFi Connected
 * @return false -> WiFi Not Connected
 */
bool WiFi_cfg::init()
{
  WiFi_cfg::ssid = WiFi_cfg::read_cfg(LittleFS, ssidPath);
  WiFi_cfg::pass = WiFi_cfg::read_cfg(LittleFS, passPath);
  WiFi_cfg::ip = WiFi_cfg::read_cfg(LittleFS, ipPath);
  WiFi_cfg::gateway = WiFi_cfg::read_cfg(LittleFS, gatewayPath);

  if (ssid == "" || ip == "")
  {
    Serial.println(PSTR("SSID or IP not defined."));
    return false;
  }

  WiFi.mode(WIFI_STA);
  localIP.fromString(ip);
  localGateway.fromString(gateway);
  subnet.fromString("255.255.0.0");

  if (!WiFi.config(localIP, localGateway, subnet, IPAddress(208, 67, 222, 222)))
  {
    Serial.println(PSTR("WiFi Configuration Error"));
    return false;
  }

  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println(PSTR("Connecting to a WiFi..."));

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    {
      Serial.println(PSTR("Connection Error"));
      retry++;
      if (retry >= 10)
        return false;
    }
    delay(1000);
  }

  Serial.println(WiFi.localIP());
  return true;
}

/**
 * @brief Creates Acces Point connection and Captive Portal Server
 *
 */
void WiFi_cfg::wifimanager()
{

  Serial.println(PSTR("AP (Access Point) Mode"));
  WiFi.softAP("ESP-WIFI-MANAGER", NULL);

  IPAddress IP = WiFi.softAPIP();
  Serial.print(PSTR("IP AP Address: "));
  Serial.println(IP);

  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(LittleFS, "/wifimanager.html", "text/html"); });
  server->serveStatic("/", LittleFS, "/");

  server->on("/", HTTP_POST, [](AsyncWebServerRequest *request)
             {
               int params = request->params();
               for (int i = 0; i < params; i++)
               {
                 AsyncWebParameter *p = request->getParam(i);
                 if (p->isPost())
                 {
                   // HTTP POST valor SSID
                   if (p->name() == "ssid")
                   {
                     ssid = p->value().c_str();
                     WiFi_cfg::write_cfg(LittleFS, ssidPath, ssid.c_str());
                   }
                   // HTTP POST valor PASS
                   if (p->name() == "pass")
                   {
                     pass = p->value().c_str();
                     WiFi_cfg::write_cfg(LittleFS, passPath, pass.c_str());
                   }
                   // HTTP POST valor IP
                   if (p->name() == "ip")
                   {
                     ip = p->value().c_str();
                     WiFi_cfg::write_cfg(LittleFS, ipPath, ip.c_str());
                   }
                   // HTTP POST valor gateway
                   if (p->name() == "gateway")
                   {
                     gateway = p->value().c_str();
                     WiFi_cfg::write_cfg(LittleFS, gatewayPath, gateway.c_str());
                   }
                 }
               }
               request->send(200, "text/plain", PSTR("Configurated at: ") + ip + PSTR(" Please reboot ESP"));
               // is_configured = true;
             });
  server->begin();
}

/**
 * @brief Read Stored WiFi Credentials
 *
 * @param fs -> File System
 * @param path -> File
 * @return String -> Value
 */
String WiFi_cfg::read_cfg(fs::FS &fs, const char *path)
{
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    Serial.println(PSTR("File reading error"));
    return String();
  }

  String fileContent;
  while (file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

/**
 * @brief Write WiFi Credentials
 *
 * @param fs  -> File System
 * @param path -> File
 * @param message
 */
void WiFi_cfg::write_cfg(fs::FS &fs, const char *path, const char *message)
{
  File file = fs.open(path, "w");
  if (!file)
  {
    Serial.println(PSTR("File writing error"));
    return;
  }
  if (file.print(message))
  {
    Serial.println("- File Saved");
  }
  else
  {
    Serial.println("- File not saved");
  }
}
