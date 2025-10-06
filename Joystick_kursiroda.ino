//arduino nano, 2 bts7960, 2 motor dc

//motor kanan
int pinpwma = 9;
int pinpwmb = 10;
//motor kiri
int pinpwmc = 5;
int pinpwmd = 6;
int speedx;
int speedx1;
void setup() {
 Serial.begin(9600);
 pinMode(pinpwma, OUTPUT);
 pinMode(pinpwmb, OUTPUT);
 pinMode(pinpwmc, OUTPUT);
 pinMode(pinpwmd, OUTPUT);
}
void loop() {
int dataadc1 = analogRead(A0);
int dataadc2 = analogRead(A1);
//Serial.print(dataadc1);
//Serial.print(" ");
//Serial.println(dataadc2);
speedx = map(dataadc1, 450, 1010, 0, 255);
speedx1 = map(dataadc2, 450, 1010, 0, 255);
if(speedx > 255){
  speedx = 255;
  }
  
if(speedx1 > 255){
  speedx1 = 255;
  }
//maju
if(speedx1 > 50){ 
  Serial.println("maju"); 
  speedx1 = speedx1 * 1;
  analogWrite(pinpwma,speedx);
  analogWrite(pinpwmb,speedx1);
  analogWrite(pinpwmc,speedx);
  analogWrite(pinpwmd,speedx1);
  }
//mundur
if(speedx1 < 0){
  Serial.println("mundur");
  speedx1 = speedx1 * -1;
  analogWrite(pinpwma,speedx1);
  analogWrite(pinpwmb,speedx);
  analogWrite(pinpwmc,speedx1);
  analogWrite(pinpwmd,speedx);
  }  
  
if(speedx > 50){
  Serial.println("kiri");
  analogWrite(pinpwma,0);
  analogWrite(pinpwmb,0);
  analogWrite(pinpwmc,0);
  analogWrite(pinpwmd,speedx);
  }
if(speedx < 0){
  Serial.println("kanan");
  speedx = speedx * -1;
  analogWrite(pinpwma,0);
  analogWrite(pinpwmb,speedx);
  analogWrite(pinpwmc,0);
  analogWrite(pinpwmd,0);
  }
if((speedx > 0)&&(speedx < 50)&&(speedx1 > 0)&&(speedx1 < 50)){ 
  analogWrite(pinpwma,0);
  analogWrite(pinpwmb,0);
  analogWrite(pinpwmc,0);
  analogWrite(pinpwmd,0);
}
      
Serial.print(speedx);
Serial.print(" ");
Serial.println(speedx1);
  
delay(100);
 
}
