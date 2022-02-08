/*https://crazymaker.com.tw/arduino-how-to-use-timer/
 *定時功能Demo 序列埠讀秒
 */
 
#include "Timer.h" 

Timer t;  //建立Timer物件
int count=0;

void WriteToSerial(){
  Serial.println(count);
  count++;
}

void setup() {
  Serial.begin(115200);
  t.every(1000,WriteToSerial); // 每1000ms呼叫WriteToSerial
}

void loop() {
  t.update(); //更新t
}
