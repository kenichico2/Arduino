#include <M5StickC.h>
#include <math.h>
#include "RoverC.h"
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

HardwareSerial VSerial(1);
TFT_eSprite tft = TFT_eSprite(&M5.Lcd);

int8_t Speed = 100;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "R_8vPXPOswSJhmHBeu5dupw5NubAwEG3"; //Auth Token for BLE

// the setup routine runs once when M5StickC starts up
void setup() {
  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen(0);

  tft.setColorDepth(8);
  tft.createSprite(80, 160);
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);

  VSerial.begin(115200, SERIAL_8N1, 33, 32);
  Wire.begin(0, 26);

  Move_stop(0);

  VSerial.write(0xAF);
  tft.fillSprite(TFT_ORANGE);
  tft.pushSprite(0, 0);

  RoverC_Init();
}

int16_t ux = 0; //16bitで符号含む。最大値は2の16乗の半分で32767
unsigned long T;
#define BASE_SPEED  20
bool last_dir = false;
uint8_t b_data[4]; //8bitで符号含まない。最大値は2の8乗で256
int16_t ux0 = 0;
int uy = 0; //おそらく16bitで符号含む。最大値は2の16乗の半分で32767
int uy0 = 0;

// the loop routine runs over and over again forever
void loop() {
  M5.update();
  double Vtuning = 0.75; //スピード調整用
  int duration = 150; //動作安定用の待ち時間
  if (M5.BtnA.wasReleased())
  {
    Move_stop(0);
    ESP.restart();
  }

  if (VSerial.available())
  {
    VSerial.write(0xAF);

    VSerial.readBytes(b_data, 4);

    // UnitVから送られたuxを受け渡し
    //ux0 = ux;
    int8_t ux = b_data[0]; //右辺は符号なしのデータなのに、なぜ左辺は符号がでるんだろう、不思議。
    

    uint32_t area = b_data[1] << 16 | b_data[2] << 8 | b_data[3];
    // おそらく上記の式で、通信のためにUnitV側で圧縮した面積の数値を復号化して面積の値としている
    // uyは前進後進のスピード。算出にはareaを使い、利用されている。遠いとスピードを速く
    
    Serial.printf("%d, %d\n", ux, area);
    if (area < 500) //認識した面積が小さい場合は検出せずと処理
    {
      if (last_dir) //おそらく右に動いてたら右回転
      {
        Move_turnright(5);
        delay(duration);
      }
      else
      {
        Move_turnleft(5);
        delay(duration);
      }
      tft.fillSprite(TFT_RED);// 液晶を赤く表示
      tft.pushSprite(0, 0);
      return;
    }
    else
    {
      tft.fillSprite(TFT_GREEN);
      tft.pushSprite(0, 0);
      if (area < 20000)
      {
        if(ux >= 0)
        {
          Move_left(ux*Vtuning);
          delay(duration);
        }
        else
        {
          Move_right(-ux*Vtuning);
          delay(duration);
        }
        uy0 = uy;
        uy = 1 / (0.00001 * area) + 15;
        uy = (uy + uy0)/2;
        if (uy > 75)
          uy = 75;
        if (uy < 5)
          uy = 5;
        
        Move_forward(uy);
        delay(duration);
      }
      else
      {
        Move_forward(20);
        delay(duration);
      }
    }

    if (ux > 0)
    {
      last_dir = true;
    }
    else
    {
      last_dir = false;
    }
  }
}
