
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2

#define LED1_PIN 8
#define LED2_PIN 10

bool led1State = false;
bool led2State = false;

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR dual LED controller ready!");
}

void loop() {
  if (IrReceiver.decode()) {

    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      uint8_t command = IrReceiver.decodedIRData.command;

      if (command == 0x07) {
        led1State = !led1State;
        digitalWrite(LED1_PIN, led1State);

        Serial.println(led1State ? "PIN 8: ON" : "PIN 8: OFF");
      }

      else if (command == 0x15) {
        led2State = !led2State;
        digitalWrite(LED2_PIN, led2State);

        Serial.println(led2State ? "PIN 10: ON" : "PIN 10: OFF");
      }
    }

    IrReceiver.resume();
  }
}
