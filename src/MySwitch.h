#ifndef _MYSWITCH_H_
#define _MYSWITCH_H_

#include <SinricProSwitch.h>

class MySwitch : public SinricProSwitch {               // create a new class "MySwitch" which inherits from SinricProSwitch
  public:
    MySwitch(const String &deviceId, int switch_pin, int button_pin);
    virtual bool onPowerState(bool &newState) override; // inherited and overloaded onPowerState (replaces the callback function)
    void checkButton();                                 // new member function to handle button press (user defined function)
  private:
    int switch_pin;                                     // pin where the output is connected to
    int button_pin;                                     // pin where the push-button is connected to
    bool powerState;                                    // variable to keep track of the powerState
    unsigned long lastMillis;                           // used for button debouncing in checkButton()
};

MySwitch::MySwitch(const String &deviceId, int switch_pin, int button_pin)
 : SinricProSwitch(deviceId)                            // initialize SinricProSwitch 
 , switch_pin(switch_pin)                               // initialize switch_pin
 , button_pin(button_pin) {                             // initialize button_pin
  pinMode(switch_pin, OUTPUT);                          // set pinMode for switch_pin to OUTPUT
  pinMode(button_pin, INPUT_PULLUP);                    // set pinMode for button_pin to INPUT_PULLUP (reading is inverted!! HIGH=released, LOW=pressed)
}

bool MySwitch::onPowerState(bool &newState) {           // same as callback-function
  powerState = newState;                                // update local powerState variable
  digitalWrite(switch_pin, newState);                   // set the pin HIGH or LOW (depending on newState)
  return true;                                          // return true -> request was properly handled
}

void MySwitch::checkButton() {                          // this function handles the button press
  if (digitalRead(button_pin)) return;                  // button pin is HIGH (not pressed)... do nothing

  unsigned long currentMillis = millis();               // }
  if (currentMillis - lastMillis <= 250) return;        //  } debounce button
  lastMillis = currentMillis;                           // }

  bool newState = !powerState;                          // newState is the invers of powerState
  onPowerState(newState);                               // use onPowerState function to switch the pin to newState
  sendPowerStateEvent(newState);                        // send Event to let SinricPro know about the change
}

#endif