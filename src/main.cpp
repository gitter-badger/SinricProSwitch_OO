#include <Arduino.h>
#include <WiFi.h>


#define SINRICPRO_OO      // use SinricPro in object orientated mode (must be defined before including SinricPro.h)
#include <SinricPro.h>
#include "MySwitch.h"

#include "credentials.h"  // rename "credentials-example.h" and change credentials to your need

#define LED1_PIN    22    // pin where LED1 is connected to
#define LED2_PIN    23    // pin where LED2 is connected to

#define BUTTON1_PIN 19    // pin where pushbutton 1 is connected to
#define BUTTON2_PIN 21    // pin where pushbutton 2 is connected to

MySwitch mySwitch1(DEVICE_ID1, LED1_PIN, BUTTON1_PIN);  // create a new instance "mySwitch1" of class MySwitch and attach LED1_PIN and BUTTON1_PIN
MySwitch mySwitch2(DEVICE_ID2, LED2_PIN, BUTTON2_PIN);  // create a new instance "mySwitch2" of class MySwitch and attach LED2_PIN and BUTTON2_PIN

void setup() {
  Serial.begin(115200);   // only needed for debug messages

  WiFi.begin(SSID, PASS);
  SinricPro.begin(APP_KEY, APP_SECRET, SINRICPRO_SERVER_URL);
}

void loop() {
  SinricPro.handle();
  mySwitch1.checkButton();
  mySwitch2.checkButton();
}
