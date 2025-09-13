#include <Arduino.h>
#include <Wire.h>
#include "BleMouse.h"
#include "Nunchuck.h"

#define SCROLL_THRESHOLD 32

// I2Cピン設定
#define SDA_PIN 8
#define SCL_PIN 9

byte z, c;
int x, y;
int scroll;

float dx_accum = 0;
float dy_accum = 0;
const float speedFactor = 0.2; // 小さくすると滑らかになる

BleMouse bleMouse;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Nunchuck.begin();
  bleMouse.begin();
}

void loop() {
  if (Nunchuck.read()) {

    x = (Nunchuck.joystick_x() - JOYSTICK_BASE) / 2;
    y = (JOYSTICK_BASE - Nunchuck.joystick_y()) / 2;

    z = Nunchuck.button_z();
    c = Nunchuck.button_c(); 

    // 小さい動きは無視
    if (abs(x) > 3 || abs(y) > 3) {

      if (z) {
        scroll -= y;
        bleMouse.move(0, 0, scroll / SCROLL_THRESHOLD); 
        scroll %= SCROLL_THRESHOLD;
      } else {
        // 累積＋スケール
        dx_accum += x * speedFactor;
        dy_accum += y * speedFactor;

        // 最大移動量制限
        dx_accum = constrain(dx_accum, -10, 10);
        dy_accum = constrain(dy_accum, -10, 10);

        // 1以上になった分だけ送信
        int moveX = (int)dx_accum;
        int moveY = (int)dy_accum;

        if (moveX != 0 || moveY != 0) {
          bleMouse.move(moveX, moveY);
          dx_accum -= moveX;
          dy_accum -= moveY;
        }
      }
    } else {
      dx_accum = 0;
      dy_accum = 0;
      scroll = 0;
    }

    // ボタン処理
    if (c) {
      if (!bleMouse.isPressed(MOUSE_RIGHT) && !bleMouse.isPressed(MOUSE_LEFT)) {
        if (z)
          bleMouse.press(MOUSE_RIGHT);
        else
          bleMouse.press(MOUSE_LEFT);
      }
    } else {
      if (bleMouse.isPressed(MOUSE_RIGHT)) 
        bleMouse.release(MOUSE_RIGHT);
      else if (bleMouse.isPressed(MOUSE_LEFT))
        bleMouse.release(MOUSE_LEFT);
    }

    // デバッグ出力
    Serial.print("x: "); Serial.print(x);
    Serial.print("\ty: "); Serial.print(y);
    Serial.print("\tz: "); Serial.print((byte)z);
    Serial.print("\tc: "); Serial.print((byte)c);    
    Serial.print("\tscroll: "); Serial.println(scroll);  
  }

  delay(10); // フレームレート向上
}
