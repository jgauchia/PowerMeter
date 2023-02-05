/**
 * @file WiFi_cfg.h
 * @author Jordi Gauchía (jgauchia@jgauchia.com)
 * @brief  ESP8266 Captive Portal Class
 * @version 0.1
 * @date 2023-01-29
 */

#ifndef WiFi_cfg_h
#define WiFi_cfg_h

#include "Arduino.h"
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWiFiManager.h> 
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <LittleFS.h>

/**
 * @brief WiFi Configuration Class definition
 * 
 */
class WiFi_cfg
{       
    public:
        WiFi_cfg();
        bool init();
        void wifimanager();

    private:
        static String ssid;
        static String pass;
        static String ip;
        static String gateway;
        IPAddress localIP;
        IPAddress localGateway;
        IPAddress subnet;
        unsigned long previousMillis = 0;
        const long wifi_interval = 10000; 
        AsyncWebServer *server;
        static const char* ssidPath;
        static const char* passPath;
        static const char* ipPath;
        static const char* gatewayPath;
  
        String read_cfg(fs::FS &fs, const char * path);
        static void write_cfg(fs::FS &fs, const char * path, const char * message);  
};

#endif