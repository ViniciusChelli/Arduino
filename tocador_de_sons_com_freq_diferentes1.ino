int pos = 0;
void setup(){ 
 pinMode(A0,INPUT); 
 pinMode(8,OUTPUT);
 pinMode(A1,INPUT); 
 pinMode(A2,INPUT);
 
}
void loop(){ 
  if(digitalRead(A0)==HIGH){ 
   tone(8,50,100);
   }
  if(digitalRead(A1)==HIGH){ 
   tone(8,950,100);  
   } 
  if(digitalRead(A2)==HIGH){ 
   tone(8,450,100); 
  } 
  delay(10);
}