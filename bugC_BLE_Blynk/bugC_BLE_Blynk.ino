#include "M5StickC.h"
#include "bugC.h"
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

char auth[] = "R_8vPXPOswSJhmHBeu5dupw5NubAwEG3"; //Auth Token for BLE



void setup()
{
  M5.begin();
  Wire.begin(0, 26, 400000);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(40, 30, 4);
  M5.Lcd.printf("BugC");
  // if add battery, need increase charge current
  M5.Axp.SetChargeCurrent(CURRENT_360MA);
  // Debug console
  Serial.begin(9600);
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("M5StickC_BugC");
  Blynk.begin(auth);
}

void loop()
{
    //ここは具体的な処理はかかない。Blynkで何かアクションがあると下の関数が呼ばれる
  Blynk.run();
}

//以下の関数はボタンがプッシュされるたびに呼ばれる
BLYNK_WRITE(V0) {
  int16_t forward = param.asInt(); //入ってくる信号を変数に渡す
  if (forward == 1) {
    BugCSetColor(0x001000, 0x001000);
    BugCSetAllSpeed(100, -100, 100, -100);
  }
  else {
    BugCSetAllSpeed(0, 0, 0, 0);
  }
}

BLYNK_WRITE(V1) {
  int16_t backward = param.asInt();
  if (backward == 1) {
    BugCSetColor(0x100000, 0x100000);
    BugCSetAllSpeed(-100, 100, -100, 100);
  }
  else {
    BugCSetAllSpeed(0, 0, 0, 0);
  }
}

BLYNK_WRITE(V2) {
  int16_t right = param.asInt();
  if (right == 1) {
    BugCSetColor(0x100000, 0x001000);
    BugCSetAllSpeed(100, 100, -100, -100);
  }
  else {
    BugCSetAllSpeed(0, 0, 0, 0);
  }
}

BLYNK_WRITE(V3) {
  int16_t left = param.asInt();
  if (left == 1) {
    BugCSetColor(0x001000, 0x100000);
    BugCSetAllSpeed(-100, -100, 100, 100);
  }
  else {
    BugCSetAllSpeed(0, 0, 0, 0);
  }
}

BLYNK_WRITE(V5) {
  int16_t rightT = param.asInt();
  if (rightT == 1) {
    BugCSetColor(0x010000, 0x010000);
    BugCSetAllSpeed(100, 100, 100, 100);
  }
  else {
    BugCSetAllSpeed(0, 0, 0, 0);
  }
}

BLYNK_WRITE(V4) {
  int16_t leftT = param.asInt();
  if (leftT == 1) {
    BugCSetColor(0x010000, 0x010000);
    BugCSetAllSpeed(-100, -100, -100, -100);
  }
  else {
    BugCSetAllSpeed(0, 0, 0, 0);
  }
}
