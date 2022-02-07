/*//https://forum.arduino.cc/t/gps-location-isupdated-condition-does-not-work/884344
*/
#include <Arduino.h>
#include <TinyGPS++.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#define RXpin 0
#define TXpin 1
  
TinyGPSPlus gps;
SoftwareSerial ss(RXpin,TXpin);
String GPS_nmea;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//全域變數這裡
String map_lon ; //longitude經度
String map_lat ; //latitude緯度


void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("begin script");

  lcd.clear();
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  Display_waiting();   //燒入完忘了接上GPS的顯示!
}

void loop(){
  while (ss.available()){
        gps.encode(ss.read());
        if (gps.location.isUpdated())//若有抓取到...
        {
          map_lat = String(gps.location.lat(),6);
          map_lon = String(gps.location.lng(),6);
          showLCD();//若取到值LCD顯示OK
          Serial.print("Latitude:");
          Serial.println((gps.location.lat(),6));
          Serial.print("Longitude:");
          Serial.println((gps.location.lng(),6));
        }
        else
        {
          GPS_nmea = ss.readStringUntil(13);
          if(GPS_nmea.startsWith("$GPGLL"))
          {
            Serial.println(GPS_nmea);
            df_LCD(); //接入GPS後的顯示
          }
        }
  }
}

void df_LCD()
{
  lcd.setCursor(0,0);  lcd.print("X:");lcd.setCursor(3,0);  lcd.print("---Wating----");
  String AAA = GPS_nmea.substring(7,17); //衛星校準前會為空值
  String BB = GPS_nmea.substring(26);
  lcd.setCursor(0,3);  lcd.print("Y:");lcd.setCursor(3,3);  lcd.print(AAA+"-"+BB);
}

//燒入完往了接上GPS的顯示!
void Display_waiting()
{  
  lcd.setCursor(3,0);
  lcd.print("Haven't GPS");
  lcd.setCursor(3,3);
  lcd.print("Pls confirm ");
}  

void showLCD()
{
  lcd.setCursor(3,0);
  //lcd.print("------OK------");
  lcd.print(String(map_lon));
  lcd.setCursor(3,3);
  //lcd.print("------OK------");
  lcd.print(String(map_lat));
}
/*
 * $GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F
$GPRMB,A,,,,,,,,,,,,V*71
$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75
$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D
$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71
$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77
$PGRME,22.0,M,52.9,M,51.0,M*14
$GPGLL,3907.360,N,12102.481,W,183730,A*33
$PGRMZ,2062,f,3*2D
$PGRMM,WGS 84*06
$GPBOD,,T,,M,,*47
$GPRTE,1,1,c,0*07
$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67
$GPRMB,A,,,,,,,,,,,,V*71
*/
