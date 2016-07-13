//http://yehnan.blogspot.tw/2013/09/arduinotower-pro-sg90.html
//0.5ms~2.0ms之間，因此0.5ms可視為負90度，2.0ms可視為正90度，
//另外要注意的是，它的死區為10us，若角度控制兩個訊號的高電位寬度差異小於10us，馬達即不會有任何變動。

#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  digitalWrite(8, HIGH);  //VCC
  digitalWrite(7, LOW);   //GND

  myservo.attach(9, 500, 2400); // 修正脈衝寬度範圍
  
  myservo.write(90); // 一開始先置中90度
  delay(500);
  myservo.write(0); // 一開始先置中90度
  delay(500);
} 

void loop() 
{
  for(int i = 500; i <= 2400; i+=100){
    myservo.writeMicroseconds(i); // 直接以脈衝寬度控制

    delay(50);
  }
  for(int i = 2400; i >= 500; i-=100){
    myservo.writeMicroseconds(i);

    delay(50);
  }
}

