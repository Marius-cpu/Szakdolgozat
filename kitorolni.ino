kitorolni


#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2
#define PIR_PIN 9

#define LED1_PIN 8
#define LED2_PIN 10

#define PIR_HOLD_TIME 15000UL

bool led1State = false;
bool led2State = false;

bool pirEnabled = true;
bool pirActive = false;
bool waitingForClear = false;

unsigned long pirStartTime = 0;

void updateLEDs() {
  digitalWrite(LED1_PIN, led1State || pirActive);
  digitalWrite(LED2_PIN, led2State || pirActive);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR + PIR controller ready!");
  Serial.println("PIR detection: ENABLED");
}

void loop() {

  // PIR motion detection
  if (pirEnabled && !pirActive && !waitingForClear) {

    if (digitalRead(PIR_PIN) == HIGH) {
      pirActive = true;
      pirStartTime = millis();

      Serial.println("MOTION DETECTED - BOTH LEDs ON");
      updateLEDs();
    }
  }

  // PIR timer: 15 seconds
  if (pirActive && millis() - pirStartTime >= PIR_HOLD_TIME) {

    pirActive = false;
    waitingForClear = true;

    Serial.println("15 SECONDS OVER - LIGHTS RELEASED");
    updateLEDs();
  }

  // Wait until PIR signal goes LOW before detecting again
  if (waitingForClear && digitalRead(PIR_PIN) == LOW) {
    waitingForClear = false;
    Serial.println("PIR READY FOR NEXT DETECTION");
  }

  // IR remote
  if (IrReceiver.decode()) {

    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      uint8_t command = IrReceiver.decodedIRData.command;

      // Button 2: LED on pin 8
      if (command == 0x07) {
        led1State = !led1State;

        Serial.println(led1State ? "PIN 8: ON" : "PIN 8: OFF");
        updateLEDs();
      }

      // Button 3: LED on pin 10
      else if (command == 0x15) {
        led2State = !led2State;

        Serial.println(led2State ? "PIN 10: ON" : "PIN 10: OFF");
        updateLEDs();
      }

      // Button 4: Toggle PIR detection
      else if (command == 0x09) {
        pirEnabled = !pirEnabled;

        if (!pirEnabled) {
          pirActive = false;
          waitingForClear = false;

          Serial.println("PIR DETECTION: DISABLED");
        } else {
          Serial.println("PIR DETECTION: ENABLED");
        }

        updateLEDs();
      }
    }

    IrReceiver.resume();
  }
}
