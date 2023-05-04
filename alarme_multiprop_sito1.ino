int led1 = 5; 
int led2 = 6; 
int led3 = 7; 
int buzzer = 2; 
int led4 = 8; 
int led5 = 9; 
int led6 = 10; 
int ledAB = 12; 
 
int temp= A1; 
int L = A0; 
 
void setup(){ 
 pinMode(L,INPUT); 
 pinMode(led1,OUTPUT); 
 pinMode(led2,OUTPUT); 
 pinMode(led3,OUTPUT); 
 pinMode(buzzer,OUTPUT); 
  
 pinMode(led4,OUTPUT); 
 pinMode(led5,OUTPUT); 
 pinMode(led6,OUTPUT); 
 pinMode(ledAB,OUTPUT);
 Serial.begin(9600); 
} 
void loop(){ 
 delay(500); 
 int SensorTemp=0; 
 SensorTemp=analogRead(temp); 
  
 float tensao = SensorTemp*(5019.9/1024.0); 
 float TemperaturaC = (tensao-500)/10;
   
 Serial.print("Valor da Temperatura = ");
 Serial.println(TemperaturaC); 
 
    
 if(TemperaturaC>=10) 
    digitalWrite(led1,HIGH); 
 else 
    digitalWrite(led1,LOW); 
   
 if(TemperaturaC>=20) 
   digitalWrite(led2,HIGH);
 else 
   digitalWrite(led2,LOW); 
   
  if(TemperaturaC>=32){ 
   digitalWrite(led3,HIGH); 
   tone(buzzer,261); 
    delay(200); 
  }
  else{ 
   digitalWrite(led3,LOW);
   noTone(buzzer);
  }  
 int ValorL=0; 
 ValorL=analogRead(L); 
 Serial.print("Valor da Luminosidade = "); 
 Serial.println(ValorL);
 Serial.println(); 
   
 if(ValorL>=600) 
   digitalWrite(led6,HIGH); 
 else 
   digitalWrite(led6,LOW);
 
  if(ValorL>=500) 
    digitalWrite(led5,HIGH); 
  else 
    digitalWrite(led5,LOW); 
  if(ValorL>=400){
    digitalWrite(led4,HIGH); 
  } 
  else{ 
    digitalWrite(led4,LOW);
    digitalWrite(ledAB,HIGH);
  } 
}

