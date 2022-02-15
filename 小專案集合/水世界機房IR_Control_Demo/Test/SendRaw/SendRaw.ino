
/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */

#include <IRremote.h>
int RECV_PIN = 2;//定义红外接收器的引脚为11
unsigned int rawCodes_ac_close[140]={8900, 4400,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,600, 600,650, 1600,600, 600,650, 550,600, 1650,650, 550,600, 550,650, 550,600, 600,650, 500,600, 600,650, 550,600, 550,650, 550,650, 1600,650, 550,650, 500,650, 550,600, 600,650, 500,650, 550,600, 1650,650, 550,650, 1600,650, 550,600, 600,650, 1600,650, 550,600, 19600,650, 550,650, 550,600, 600,600, 550,650, 550,650, 550,600, 550,650, 550,600, 600,600, 550,600, 600,650, 550,600, 550,650, 1600,650, 550,650, 550,650, 500,650, 550,650, 550,600, 550,650, 550,650, 550,650, 500,650, 550,600, 600,600, 600,600, 550,650, 550,650, 1600,600, 600,650, 1600,650, 550,600, 3440};
unsigned int rawCodes_ac_open[140]={8850, 4400,650, 550,600, 550,600, 600,650, 550,600, 550,650, 550,650, 550,600, 550,600, 1650,650, 550,600, 600,650, 1600,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 1650,600, 550,650, 550,600, 550,650, 550,650, 550,650, 500,650, 1650,550, 600,650, 1650,600, 550,650, 550,650, 1600,650, 550,650, 19550,700, 550,600, 550,650, 550,650, 550,600, 550,650, 550,600, 600,550, 600,650, 550,650, 550,600, 550,650, 550,650, 550,600, 1650,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,650, 550,600, 550,650, 550,600, 600,600, 550,650, 1600,650, 550,650, 1600,650, 550,650,15264};//上面改成你接受到的数据，请注意把接受到的第一个数据放到最后

IRrecv irrecv(RECV_PIN);
decode_results results;
IRsend  irsend;

void setup()
{
  Serial.begin(9600);
 irrecv.enableIRIn(); // 初始化红外接收器
}


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
    Serial.print(results->panasonicAddress,HEX);
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


void loop() {
  //irsend.sendRaw(rawCodes_ac_close,140, 38);
  irsend.sendRaw(rawCodes_ac_open,140, 38);
  delay(5);

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    dump(&results);
 
    irrecv.resume(); // Receive the next value
  }
   delay(5000);
}
