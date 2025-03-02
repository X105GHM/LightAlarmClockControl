#include "HTTPManager.h"
#include <HTTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include "credentials.h"

HTTPManager::HTTPManager(){}

HTTPManager::~HTTPManager() {}

void HTTPManager::connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to WiFi...");

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) // 10 Sekunden Timeout
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nConnected to WiFi!");
    }
    else
    {
        Serial.println("\nWiFi connection failed!");
    }
}

void HTTPManager::sendDisplayData(const std::string &data)
{
    HTTPClient http;

    if (!http.begin("http://192.168.178.130/update"))
    {
        Serial.println("Error: Invalid URL");
        return;
    }

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
    std::string url = "http://192.168.178.189/set";
    std::string payload = on ? "{\"state\":\"on\"}" : "{\"state\":\"off\"}";

    Serial.print("Sende HTTP-POST an ");
    Serial.println(url.c_str());
    Serial.print("Payload: ");
    Serial.println(payload.c_str());

    http.begin(url.c_str());
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload.c_str());

    if (httpResponseCode > 0)
    {
        Serial.printf("HTTP-Antwort: %d\n", httpResponseCode);
    }
    else
    {
        Serial.printf("Fehler beim Senden des Befehls: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}

void HTTPManager::sendDirectOn()
{
    HTTPClient http;
    std::string url = "http://192.168.178.189/On";
    std::string payload = "{\"command\":\"Direct On\"}";

    Serial.print("Sende Direct On an ");
    Serial.println(url.c_str());
    Serial.print("Payload: ");
    Serial.println(payload.c_str());

    http.begin(url.c_str());
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload.c_str());

    if (httpResponseCode > 0)
    {
        Serial.printf("HTTP-Antwort: %d\n", httpResponseCode);
    }
    else
    {
        Serial.printf("Fehler beim Senden des Direct On-Befehls: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}

void HTTPManager::sendDirectOff()
{
    HTTPClient http;
    std::string url = "http://192.168.178.189/stop";
    std::string payload = "{\"command\":\"Direct off\"}";

    Serial.print("Sende Direct Off an ");
    Serial.println(url.c_str());
    Serial.print("Payload: ");
    Serial.println(payload.c_str());

    http.begin(url.c_str());
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload.c_str());

    if (httpResponseCode > 0)
    {
        Serial.printf("HTTP-Antwort: %d\n", httpResponseCode);
    }
    else
    {
        Serial.printf("Fehler beim Senden des Direct Off-Befehls: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}