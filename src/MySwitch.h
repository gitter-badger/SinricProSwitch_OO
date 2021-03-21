#ifndef _MYSWITCH_H_
#define _MYSWITCH_H_

#include <SinricProSwitch.h>                            // this device is based on SinricProSwitch...so include it
#include <FunctionalInterrupt.h>                        // provide std::bind to bind member functions to interrupt routines (ISR)

class MySwitch : public SinricProSwitch {               // create a new class "MySwitch" which inherits from SinricProSwitch
  public:
    MySwitch(const String &deviceId, int led_pin, int button_pin);
    virtual bool onPowerState(bool &newState) override; // inherited and overloaded onPowerState (replaces the callback function)

  private:
    int led_pin;                                        // pin where the output is connected to
    int button_pin;                                     // pin where the push-button is connected to
    bool powerState;                                    // variable to keep track of the powerState
    unsigned long lastMillis;                           // used for button debouncing in handleButtonISR()
    
    void handleButtonISR();                             // user defined (ISR) function to handle button press
};

MySwitch::MySwitch(const String &deviceId, int led_pin, int button_pin)
 : SinricProSwitch(deviceId)                            // initialize SinricProSwitch 
 , led_pin(led_pin)                                     // initialize switch_pin
 , button_pin(button_pin) {                             // initialize button_pin
  pinMode(led_pin, OUTPUT);                             // set pinMode for switch_pin to OUTPUT
  pinMode(button_pin, INPUT_PULLUP);                    // set pinMode for button_pin to INPUT_PULLUP (reading is inverted!! HIGH=released, LOW=pressed)
  attachInterrupt(                                      // attach interrupt
    button_pin,                                         //   pin wich triggers the interrupt
    std::bind(&MySwitch::handleButtonISR, this),        //   bind this `handleButtonISR` to the interrupt
    FALLING                                             //   when signal is FALLING EDGE
  );  
}

bool MySwitch::onPowerState(bool &newState) {           // same as callback-function (except deviceId <- not necessary anylonger because "we are" the device!)
  powerState = newState;                                // update powerState variable
  digitalWrite(led_pin, newState);                      // set the pin HIGH or LOW (depending to newState)
  return true;                                          // return true -> request was properly handled
}

void MySwitch::handleButtonISR() {                      // this (ISR) function handles the button press
  unsigned long currentMillis = millis();               // .
  if (currentMillis - lastMillis <= 250) return;        //  . debounce button
  lastMillis = currentMillis;                           // .
 
  bool newState = !powerState;                          // newState is the invers of powerState
  onPowerState(newState);                               // call onPowerState with newState to do the real work (digitalWrite..)
  sendPowerStateEvent(newState);                        // send event to SinricPro Server with new state
}

#endif