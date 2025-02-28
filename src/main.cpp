#include <Arduino.h>
#include <EEPROM.h>
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

RotaryEncoder encoder(ROTARY_PIN_A, ROTARY_PIN_B, ROTARY_BUTTON_PIN);
Button confirmButton(CONFIRM_BUTTON_PIN);
Button snoozeButton(SNOOZE_BUTTON_PIN);
SwitchInput alarmSwitch(SWITCH_PIN);
Led signalLed(LED_PIN);
HTTPManager httpManager;
NTPManager ntpManager("de.pool.ntp.org", 3600, 3600);
AlarmClock alarmClock(encoder, confirmButton, snoozeButton, alarmSwitch, signalLed, httpManager, ntpManager);

void inputTask(void *pvParameters)
{
  AlarmClock *alarm = static_cast<AlarmClock *>(pvParameters);
  while (true)
  {
    alarm->processInputs();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  vTaskDelete(NULL);
}

void httpTask(void *pvParameters)
{
  AlarmClock *alarm = static_cast<AlarmClock *>(pvParameters);
  while (true)
  {
    alarm->processHTTP();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);

  xTaskCreate
  (
      inputTask,   
      "InputTask", 
      4096,        
      &alarmClock, 
      1,           
      NULL         
  );

  xTaskCreate
  (
      httpTask,    
      "HTTPTask",  
      4096,        
      &alarmClock, 
      1,           
      NULL         
  );

  NTPManager ntpManager;

  if (ntpManager.updateLocalTime())
  {
    Serial.println("NTP-Zeit erfolgreich aktualisiert.");
  }
  else
  {
    Serial.println("NTP-Zeit konnte nicht aktualisiert werden.");
  }

  vTaskDelete(NULL);
}

void loop(){};