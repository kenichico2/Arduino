#include <M5StickC.h>
#include "RoverC.h"
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

int8_t Speed = 100;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = ""; //Auth Token for BLE

// the setup routine runs once when M5StickC starts up
void setup() {

  // Initialize the M5StickC object
  M5.begin();

  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(40, 30, 4);
  M5.Lcd.printf("BlynkBLE");

  // Debug console
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("M5StickC_RoverC");
  Blynk.begin(auth);

  RoverC_Init();
}

// the loop routine runs over and over again forever
void loop() {
  //ここは具体的な処理はかかない。Blynkで何かアクションがあると下の関数が呼ばれる
  Blynk.run();

}

//以下の関数はボタンがプッシュされるたびに呼ばれる
BLYNK_WRITE(V0) {
  int16_t forward = param.asInt(); //入ってくる信号を変数に渡す
  if (forward == 1) {
    Move_forward(Speed);
  }
  else {
    Move_stop(0);
  }
}

BLYNK_WRITE(V1) {
  int16_t backward = param.asInt();
  if (backward == 1) {
    Move_back(Speed);
  }
  else {
    Move_stop(0);
  }
}

BLYNK_WRITE(V2) {
  int16_t right = param.asInt();
  if (right == 1) {
    Move_right(Speed);
  }
  else {
    Move_stop(0);
  }
}

BLYNK_WRITE(V3) {
  int16_t left = param.asInt();
  if (left == 1) {
    Move_left(Speed);
  }
  else {
    Move_stop(0);
  }
}

BLYNK_WRITE(V4) {
  int16_t rightT = param.asInt();
  if (rightT == 1) {
    Move_turnright(Speed);
  }
  else {
    Move_stop(0);
  }
}

BLYNK_WRITE(V5) {
  int16_t leftT = param.asInt();
  if (leftT == 1) {
    Move_turnleft(Speed);
  }
  else {
    Move_stop(0);
  }
}
