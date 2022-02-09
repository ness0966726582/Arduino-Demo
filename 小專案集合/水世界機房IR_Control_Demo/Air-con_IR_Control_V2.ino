/*開發人員:Ness
 *開發日期:20220209
 *用途:針對水世界無法自動復歸的冷氣進行自動控制
 *
 */








#include "DHT.h"
DHT dht;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <IRremote.h>
int RECV_PIN = 2; // 使用數位腳位2接收紅外線訊號
IRrecv irrecv(RECV_PIN); // 初始化紅外線訊號輸入
decode_results results; // 儲存訊號的結構
IRsend irsend; // IRRemote限定使用數位腳位3

int sw_pin = 4;

void setup() {
  Serial.begin(115200);

  irrecv.blink13(true); // 設為true的話，當收到訊號時，腳位13的LED便會閃爍
  irrecv.enableIRIn(); // 啟動接收
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);  lcd.print("Hello, world!");
  lcd.setCursor(2,1);  lcd.print("Arduino!");
  dht.setup(5); 
  pinMode(sw_pin,INPUT);  //SW_Pin
}

void Read_IR()
{
  if (irrecv.decode(&results)) { // 接收紅外線訊號並解碼
    lcd.clear();
    lcd.setCursor(2,0);  lcd.print("IR VALUE");
    lcd.setCursor(2,1);  lcd.print(results.value, HEX);
    Serial.println(results.value, HEX);
    irrecv.resume(); // 準備接收下一個訊號
  }
}

void wirte()
{ 
  unsigned long v = 0x0;
  v = 0xB24D5FA0;
  Serial.println(v, HEX);
  irsend.sendNEC(v, 32); // 輸出紅外線訊號
  
  lcd.clear();
  lcd.setCursor(2,0);  lcd.print("IR SENT");
  lcd.setCursor(2,1);  lcd.print(v, HEX);
  delay(5000);  //10秒偵測冷氣
  lcd.setCursor(2,1);  lcd.print("10S Wating------");
  delay(20000);  //10秒偵測冷氣
}

void SW()
{ 
  bool val = digitalRead(sw_pin);
  if (val==true)
  { irrecv.disableIRIn();//關閉IR讀取
    delay(dht.getMinimumSamplingPeriod());
    int temp = dht.getTemperature();
    Serial.print("ON-");   
    Serial.println(temp);
    
    if (temp>26)
    { Serial.print("ON-");   
      Serial.println(temp);
      wirte();
    }
    else
    { 
      lcd.clear();
      lcd.setCursor(2,0);  lcd.print("Power:ON");
      lcd.setCursor(2,1);  lcd.print("Air-con OK!");
      delay(1000);
    }
  }else{ 
    Read_IR();
    } 
}

void loop() {
  SW();
}
