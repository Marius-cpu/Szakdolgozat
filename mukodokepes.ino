#MUKODOKEPES, INNEN LEHET KIINDULNI

#include <IRremote.hpp>

// ---------- Pins ----------
#define IR_RECEIVE_PIN 2
#define LED2_PIN       7
#define LED1_PIN       8
#define LED3_PIN       9
#define RELAY_PIN      12

// ---------- Remote buttons (NEC) ----------
#define IR_1 0x45
#define IR_2 0x46
#define IR_3 0x47
#define IR_4 0x44
#define IR_5 0x40
#define IR_6 0x43
#define IR_7 0x7
#define IR_8 0x15
#define IR_9 0x9

// ---------- Button assignments ----------
#define BTN_LED1  IR_1   // LED 1, pin 8
#define BTN_LED2  IR_2   // LED 2, pin 7
#define BTN_RELAY IR_3   // relay, pin 12
#define BTN_LED3  IR_4   // LED 3, pin 10

bool led1State = false;
bool led2State = false;
bool led3State = false;
bool relayState = false;

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR controller ready!");
}

void loop() {
  if (IrReceiver.decode()) {

    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      uint8_t command = IrReceiver.decodedIRData.command;

      Serial.print("Received command: 0x");
      Serial.println(command, HEX);

      switch (command) {

        case BTN_LED1:
          led1State = !led1State;
          digitalWrite(LED1_PIN, led1State);
          Serial.println(led1State ? "LED 1 (pin 8): ON" : "LED 1 (pin 8): OFF");
          break;

        case BTN_LED2:
          led2State = !led2State;
          digitalWrite(LED2_PIN, led2State);
          Serial.println(led2State ? "LED 2 (pin 7): ON" : "LED 2 (pin 7): OFF");
          break;

        case BTN_RELAY:
          relayState = !relayState;
          digitalWrite(RELAY_PIN, relayState);
          Serial.println(relayState ? "RELAY (pin 12): ON" : "RELAY (pin 12): OFF");
          break;

        case BTN_LED3:
          led3State = !led3State;
          digitalWrite(LED3_PIN, led3State);
          Serial.println(led3State ? "LED 3 (pin 10): ON" : "LED 3 (pin 10): OFF");
          break;

        default:
          Serial.println("Unassigned button");
          break;
      }
    }

    IrReceiver.resume();
  }
}
