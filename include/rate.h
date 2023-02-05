/**
 * @file rate.h
 * @author Jordi Gauchía (jgauchia@jgauchia.com)
 * @brief  Electricity Rate functions
 * @version 0.1
 * @date 2023-02-03
 */

time_t now;
tm tm;

/**
 * @brief Electricity Web API server
 *
 */
String serverName = "https://api.preciodelaluz.org/v1/prices/all?zone=PCB";

/**
 * @brief JSON deserialize document
 *
 */
DynamicJsonDocument tarifa(7000);

/**
 * @brief Struct definition for JSON data parsing
 *
 */
typedef struct data
{
    char value_hour[7];
    float value_price;
    bool value_is_cheap;
    bool value_is_under_avg;
};

/**
 * @brief Array for JSON data storage
 *
 */
data JSONdata[24];

/**
 * @brief Get the current day rate
 *
 */
void get_current_rate()
{
    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure();
    http.begin(client, serverName.c_str());
    int httpResponseCode = http.GET();

    int hour = 0;

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();

        DeserializationError error = deserializeJson(tarifa, payload);

        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }

        for (JsonPair item : tarifa.as<JsonObject>())
        {
            // const char *item_key = item.key().c_str();              // "00-01", "01-02", "02-03", "03-04", "04-05", "05-06", ...
            // const char *value_date = item.value()["date"];          // "30-01-2023", "30-01-2023", "30-01-2023", ...
            // char *value_hour = item.value()["hour"];                // "00-01", "01-02", "02-03", "03-04", "04-05", "05-06", ...
            // bool value_is_cheap = item.value()["is-cheap"];         // true, true, false, true, true, true, true, false, ...
            // bool value_is_under_avg = item.value()["is-under-avg"]; // true, true, true, true, true, true, true, ...
            // const char *value_market = item.value()["market"];      // "PVPC", "PVPC", "PVPC", "PVPC", "PVPC", "PVPC", ...
            // float value_price = item.value()["price"];              // 169.49, 173.02, 173.99, 164.41, 140.8, 173.43, 168.96, ...
            // const char *value_units = item.value()["units"];        // "€/Mwh", "€/Mwh", "€/Mwh", "€/Mwh", "€/Mwh", ...

            strcpy(JSONdata[hour].value_hour, item.value()["hour"]);
            JSONdata[hour].value_price = item.value()["price"];
            JSONdata[hour].value_price = (JSONdata[hour].value_price / 1000);
            JSONdata[hour].value_is_cheap = item.value()["is-cheap"];
            JSONdata[hour].value_is_under_avg = item.value()["is-under-avg"];
            hour++;
        }
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}