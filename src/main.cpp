#include <Arduino.h>
#include <ESP8266WiFi.h>

#define SINRICPRO_OO // https://github.com/sinricpro/esp8266-esp32-sdk/tree/dev-oop !!
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define SSID                ""
#define PASS                ""

#define APP_KEY             ""
#define APP_SECRET          ""

#define RELAY_ID_1          ""
#define RELAY_ID_2          ""
#define RELAY_ID_3          ""
#define RELAY_ID_4          ""  

#define RELAY_PIN_1         D4
#define RELAY_PIN_2         D5
#define RELAY_PIN_3         D6
#define RELAY_PIN_4         D7

class Relay : public SinricProSwitch {
  public:
    Relay(const String &deviceId, int gpio) : SinricProSwitch(deviceId), gpio(gpio) { pinMode(gpio, OUTPUT); }
  protected:

    bool onPowerState(bool &state) override {
      Serial.printf("Relay \"%s\" on gpio %d turned %s\r\n", deviceId.toString().c_str(), gpio, state? "on" : "off");
      digitalWrite(gpio, state);
      return true;
    };

    void loop() override { 
      unsigned long currentMillis = millis();
      if (currentMillis - lastMessage < 5000) return;
      lastMessage = currentMillis;
      Serial.printf("Hello from relay \"%s\" who controls gpio %d\r\n", deviceId.toString().c_str(), gpio);
    }

    int gpio;
    unsigned long lastMessage;
};

// relays on stack
Relay relay1_on_stack(RELAY_ID_1, RELAY_PIN_1);
Relay relay2_on_stack(RELAY_ID_2, RELAY_PIN_2);

// relay on heap
Relay *relay3_on_heap = new Relay(RELAY_ID_3, RELAY_PIN_3);
// relay 4 -> see setup!

void setup() {
  Serial.begin(115200);                                

  WiFi.begin(SSID, PASS);
  while (!WiFi.isConnected()) { 
    Serial.printf(".");
    delay(100);
  }
  Serial.printf("IP: %s\r\n", WiFi.localIP().toString().c_str());

  // create a new relay on heap... and forget about it...but it will still remain on the heap and keep running ;)
  new Relay(RELAY_ID_4, RELAY_PIN_4); 

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  SinricPro.handle();
}
