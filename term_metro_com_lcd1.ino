#include <LiquidCrystal.h> 
 
LiquidCrystal lcd(12,11,5,4,3,2); 
const int temp = A4; 
float temperatura; 
float celsius;
float conversor; 
 
void setup(){ 
 Serial.begin(9600); 
 lcd.begin(16,2); 
 lcd.print("Temperatura!"); 
} 
void loop(){ 
 temperatura = analogRead(temp); 
 conversor = temperatura*5; 
 conversor = conversor/1024; 
 celsius = (conversor - 0.5)*100; 
 Serial.println(celsius); 
 lcd.setCursor(0,0); 
 lcd.print("Temperatura:"); 
 lcd.setCursor(0,1); 
 lcd.print(celsius);
   
  
}