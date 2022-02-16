/*開發人員:Ness
 *開發日期:2022-02-16
 *開發目的說明:
 *
 *-讀取遙控器IR-->顯示於Serial port
 *-偵測溫度判斷>25度 -->發送IR
 *
*/

#include "DHT.h"
DHT dht;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <IRremote.h>
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;
IRsend  irsend;

//自訂義Air-con rawCodes，取得的第一个數值放到最後
String _Brand_ = "Kolin";
unsigned int rawCodes_ac_close[140]={8900, 4400,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,600, 600,650, 1600,600, 600,650, 550,600, 1650,650, 550,600, 550,650, 550,600, 600,650, 500,600, 600,650, 550,600, 550,650, 550,650, 1600,650, 550,650, 500,650, 550,600, 600,650, 500,650, 550,600, 1650,650, 550,650, 1600,650, 550,600, 600,650, 1600,650, 550,600, 19600,650, 550,650, 550,600, 600,600, 550,650, 550,650, 550,600, 550,650, 550,600, 600,600, 550,600, 600,650, 550,600, 550,650, 1600,650, 550,650, 550,650, 500,650, 550,650, 550,600, 550,650, 550,650, 550,650, 500,650, 550,600, 600,600, 600,600, 550,650, 550,650, 1600,600, 600,650, 1600,650, 550,600, 3440};
unsigned int rawCodes_ac_open[140]={8850, 4400,650, 550,600, 550,600, 600,650, 550,600, 550,650, 550,650, 550,600, 550,600, 1650,650, 550,600, 600,650, 1600,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 1650,600, 550,650, 550,600, 550,650, 550,650, 550,650, 500,650, 1650,550, 600,650, 1650,600, 550,650, 550,650, 1600,650, 550,650, 19550,700, 550,600, 550,650, 550,650, 550,600, 550,650, 550,600, 600,550, 600,650, 550,650, 550,600, 550,650, 550,650, 550,600, 1650,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,600, 600,600, 550,650, 1600,650, 550,650, 1600,650, 550,650,15264};

//+=============================================================================
void setup() {
  Serial.begin(9600);
  irrecv.blink13(true); // 設為true的話，當收到訊號時，腳位13的LED便會閃爍
  irrecv.enableIRIn(); // 發送模式時請//irrecv.enableIRIn()這個函式
  irrecv.resume();

  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);  lcd.print("Hello!");
  lcd.setCursor(2,1);  lcd.print("Air-con");
  
  dht.setup(5); 

  
}

//+=============================================================================
//自訂義dump: Serial port show data
void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } 
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } 
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  } 
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  } 
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {  
    Serial.print("Decoded PANASONIC - Address: ");
    //Serial.print(results->panasonicAddress,HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == JVC) {
     Serial.print("Decoded JVC: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}

//+=============================================================================
//自訂義IR_RECV:Serial port show data
void IR_RECV(){
  lcd.clear();
  lcd.setCursor(2,0);  lcd.print("Try your IR");
  lcd.setCursor(2,1);  lcd.print("IR waiting...");
  delay(1000);
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    lcd.clear();
    lcd.setCursor(2,0);  lcd.print("Your IR codes:");
    lcd.setCursor(2,1);  lcd.print(results.value, HEX);
    delay(2000);
    dump(&results);
    irrecv.resume(); // Receive the next value
    delay(5);
  }
}

//+=============================================================================
//自訂義IR_Send:sending your IR rawCodes
void IR_SEND(){
  lcd.clear();
  lcd.setCursor(0,0);  lcd.print(_Brand_);
  lcd.setCursor(0,1);  lcd.print("IR: Sending...");
  irsend.sendRaw(rawCodes_ac_open,140, 38);
  delay(5);
}

void CHECK_TEMP(){
  delay(dht.getMinimumSamplingPeriod());
    int temp = dht.getTemperature();
    Serial.println(temp);
    delay(1000);
    if (temp>25){      
      Serial.println("TEMP>25 啟動");
      IR_SEND();
    }else{
      lcd.clear();
      lcd.setCursor(0,0);  lcd.print(_Brand_);
      lcd.setCursor(0,1);  lcd.print("TEMP is normal");      
      Serial.println("TEMP<25 維持");
     }delay(2000);

}
void loop() {
    //CHECK_TEMP();//檢查溫度發送IR 1.修改上方RAW碼 2.因衝突DHT 請修改上方設置區,註記//irrecv.enableIRIn()
    IR_RECV();
  
}
