/**
 * @file main.cpp
 * @author Jordi Gauchía (jgauchia@jgauchia.com)
 * @brief
 * @version 0.1
 * @date 2023-01-29
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESPAsyncWiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <LittleFS.h>
#include <time.h>
#include <OneButton.h>
#include <ArduinoJson.h>
#include "WiFi_cfg.h"
#include "rate.h"
#include "buttons.h"
#include "gui.h"

#define NTP "hora.cica.es"
#define TZ "CET-1CEST,M3.5.0,M10.5.0/3"

WiFi_cfg wifi;

void setup()
{
  Serial.begin(115200);

  init_buttons();
  configTime(TZ, NTP);

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.clearBuffer();
  u8g2.drawStr(ALIGN_CENTER("CONNECTING..."), 20, "CONNECTING...");
  u8g2.sendBuffer();

  if (wifi.init())
  {
    u8g2.clearBuffer();
    u8g2.drawStr(ALIGN_CENTER("CONNECTED"), 10, "CONNECTED");
    u8g2.drawStr(ALIGN_CENTER(WiFi.localIP().toString().c_str()), 20, WiFi.localIP().toString().c_str());
    u8g2.sendBuffer();
    delay(2000);

    time(&now);
    localtime_r(&now, &tm);

    get_current_rate();

    current_hour = tm.tm_hour;
    old_hour = tm.tm_hour;

    update_oled();
  }
  else
  {
    wifi.wifimanager();
    u8g2.clearBuffer();
    u8g2.drawStr(ALIGN_CENTER("NOT CONNECTED"), 10, "NOT CONNECTED");
    u8g2.drawStr(ALIGN_CENTER("CONFIGURE WIFI"), 20, "CONFIGURE WIFI");
    u8g2.sendBuffer();
  }
}

void loop()
{

  get_buttons();

  time(&now);
  localtime_r(&now, &tm);

  if (tm.tm_hour != old_hour)
  {
    current_hour = tm.tm_hour;
    old_hour = tm.tm_hour;
  }

  update_oled();
  // delay(1000);
}