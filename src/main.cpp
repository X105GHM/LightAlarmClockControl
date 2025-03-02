#include <Arduino.h>
#include <EEPROM.h>
#include <Preferences.h>
#include "RotaryEncoder/RotaryEncoder.h"
#include "Button/Button.h"
#include "SwitchInput/Switchinput.h"
#include "Led/LED.h"
#include "HTTPManager/HTTPManager.h"
#include "AlarmClock/AlarmClock.h"
#include "NTPManager/NTPManager.h"

constexpr int ROTARY_PIN_A = 34;
constexpr int ROTARY_PIN_B = 35;
constexpr int ROTARY_BUTTON_PIN = 32;
constexpr int CONFIRM_BUTTON_PIN = 33;
constexpr int SNOOZE_BUTTON_PIN = 25;
constexpr int SWITCH_PIN = 26;
constexpr int LED_PIN = 27;

Preferences preferences;
RotaryEncoder encoder(ROTARY_PIN_A, ROTARY_PIN_B, ROTARY_BUTTON_PIN);
Button button(CONFIRM_BUTTON_PIN, SNOOZE_BUTTON_PIN);
SwitchInput alarmSwitch(SWITCH_PIN);
Led signalLed(LED_PIN);
HTTPManager httpManager;
NTPManager ntpManager("de.pool.ntp.org", 3600, 3600);
AlarmClock alarmClock(encoder, button, alarmSwitch, signalLed, httpManager, ntpManager, preferences);


void inputTask(void *pvParameters)
{
  AlarmClock *alarm = static_cast<AlarmClock *>(pvParameters);
  while (true)
  {
    alarm->processInputs();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void httpTask(void *pvParameters)
{
  AlarmClock *alarm = static_cast<AlarmClock *>(pvParameters);
  while (true)
  {
    alarm->processHTTP();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void setup()
{
  Serial.begin(115200);

  Serial.println("EEPROM initialisiert.");

  httpManager.connectWiFi();

  xTaskCreatePinnedToCore
  (
      inputTask,   
      "InputTask", 
      4096,        
      &alarmClock, 
      1,           
      NULL,
      0         
  );

  Serial.println("InputTask initialisiert.");

  xTaskCreatePinnedToCore
  (
      httpTask,    
      "HTTPTask",  
      4096,        
      &alarmClock, 
      1,           
      NULL,
      1         
  );

  Serial.println("HTTPTask initialisiert.");

  NTPManager ntpManager;

  if (ntpManager.updateLocalTime())
  {
    Serial.println("NTP-Zeit erfolgreich aktualisiert.");
  }
  else
  {
    Serial.println("NTP-Zeit konnte nicht aktualisiert werden.");
  }

    preferences.begin("alarmClock", false);
    alarmClock.loadAlarmFromPreferences();

  vTaskDelete(NULL);
}

void loop(){};