//LCD
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

//Czujnik IR
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremoteInt.h>
#include <boarddefs.h>
#include <IRremote.h>

#define irPin 11
IRrecv irrecv(irPin);
decode_results results;
int n = 1;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  lcd.begin (16,2);

  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  Serial.begin(9600);
  irrecv.enableIRIn();
  
  lcd.print("Czesc!");
  delay(3000);
  lcd.print("");
}
  


void loop() {
  lcd.clear();
  lcd.print("Cykle:");
  delay(1500);
  int cycles = control();
  
  lcd.clear();
  lcd.print("Ilosc cykli: ");
  lcd.print(cycles);
  delay(2000);
  
  lcd.clear();
  lcd.print("Mruganie:");
  delay(1500);
  int blinkTime = control();
  
  lcd.clear();
  lcd.print("Mrug czas:");
  lcd.print(blinkTime);
  delay(2000);
  
  lcd.clear();
  lcd.print("Przerwa:");
  delay(1500);
  int breakTime = control();
  
  lcd.clear();
  lcd.print("Przerwy to:");
  lcd.print(breakTime);
  delay(2000);
  
  lcd.clear();
  
  blinkCycle(cycles, blinkTime, breakTime);
  delay(2000);
}


long read() {
  irrecv.resume();
  if (irrecv.decode(&results)) {
        return results.value;
      } 
}



void blinkCycle(int cycles, int blinkTime, int breakTime)
{
 do {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(blinkTime * 1000);     
    checkExit(); 
    digitalWrite(LED_BUILTIN, LOW);   
    delay(breakTime * 1000);
    checkExit(); 
    lcd.clear();
    lcd.print("Pozostalo cykli:");
    lcd.setCursor(0,1);
    lcd.print(cycles);    
    cycles--;
   }while (cycles >= 0);
  
  }

void checkExit() {
  if(irrecv.decode(&results)) {
      if(results.value==867696895) {
          lcd.clear();
          digitalWrite(LED_BUILTIN, LOW);
          lcd.print("ZEGNAJ");
          delay(5000);
          exit(0);
      }
      irrecv.resume();
    }      
}
int control() {
  long value;
  if(irrecv.decode(&results)) {
        value = results.value;
        long returnValue;
       // Serial.println(value);
        delay(2000);
        lcd.clear();
        switch(value)
        {
          case 867698935:
            returnValue = 0;
            break;
          case 867731575:
            returnValue = 1;
            break;
          case 867715255:
            returnValue = 2;
            break;
          case 867747895:
            returnValue = 3;
            break;
          case 867707095:
            returnValue = 4;
            break;
          case 867739735:
            returnValue = 5;
            break;
          case 867723415:
            returnValue = 6;
            break;
          case 867756055:
            returnValue = 7;
            break;
          case 867703015:
            returnValue = 8;
            break;
          case 867735655:
            returnValue = 9;
            break;
          case 867696895:
            lcd.print("ZEGNAJ");
            delay(5000);
            exit(0);
            break;
          default:
          returnValue = 101;
        }
        irrecv.resume();
        return returnValue;
      }
 
}

//0 - 33B808F7  // 867698935
//1 - 33B88877  // 867731575 
//2 - 33B848B7  // 867715255
//3 - 33B8C837  // 867747895 
//4 - 33B828D7  // 867707095
//5 - 33B8A857  // 867739735  
//6 - 33B86897  // 867723415
//7 - 33B8E817  // 867756055
//8 - 33B818E7  // 867703015
//9 - 33B89867  // 867735655
//FFFFFFF POWTORZENIE 4294967295


