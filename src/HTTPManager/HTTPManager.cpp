#include "HTTPManager.h"
#include <HTTPClient.h>
#include <Arduino.h>
#include <WiFi.h>

HTTPManager::HTTPManager()
{
    WiFi.begin("FRITZ!Box 7490", "Isaf2009");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

HTTPManager::~HTTPManager() {}

void HTTPManager::sendDisplayData(const std::string &data)
{
    HTTPClient http;
    http.begin("http://192.168.1.100/update");
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(data.c_str());
    if (httpResponseCode > 0)
    {
        Serial.printf("Display update response: %d\n", httpResponseCode);
    }
    else
    {
        Serial.printf("Error sending display data: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
}

void HTTPManager::sendLightCommand(bool on)
{
    HTTPClient http;
    http.begin("http://192.168.1.101/set");
    http.addHeader("Content-Type", "application/json");
    std::string payload = on ? "{\"state\": \"on\"}" : "{\"state\": \"off\"}";
    int httpResponseCode = http.POST(payload.c_str());
    if (httpResponseCode > 0)
    {
        Serial.printf("Light command response: %d\n", httpResponseCode);
    }
    else
    {
        Serial.printf("Error sending light command: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
}
