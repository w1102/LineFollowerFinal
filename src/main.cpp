#include <Arduino.h>
#include <Servo.h> 
#include "StateMachine.h"
#include "PID.h"

// PID parameter for sensor. chỉ dùng khâu tỷ lệ
float kp = 7.5;
float ki = 0;
float kd = 0;

int IN[2] = {9, 8}; // input pin l298

#define motorPin 5 // anable A 
#define servoPin 3 // PWM pin

int minSpeed = 100;
int maxSpeed = 180;
int maxLeftdegree = 10;     // góc servo trái
int maxRightdegree = 110;   // góc servo phải
int centerDegree = 80;      // góc servo giữa

int bias; // vị trí xe so với line 
int prebias; // vị trí trước đó

bool firstRA = 1; // góc vuông đầu tiên
unsigned long timeGet;

Servo myservo;

StateMachine myState(&bias, &prebias); 

PID mySensorPID(&kp, &ki, &kd);




void RAProcess(); // hàm quẹo góc vuông

void setup() {
  Serial.begin(115200);

  pinMode(IN[0],OUTPUT);//  l298 input pin 
  pinMode(IN[1],OUTPUT);//  l298 input pin
  pinMode(motorPin, OUTPUT); //   motor pin PWM
  myservo.attach(servoPin); //  servo pin

}











void loop() {
  //đọc trạng thái xe
  bool rightAngle = myState.getState(); 
  // trả về true cho rightAngle nếu là góc vuông
  // vị trí xe đc trả về bias qua con trỏ.

  // xử lý góc vuông
  if (firstRA == 1 or ( (unsigned long) (millis() - timeGet) > 8000) )  {
  // nếu là góc vuông đầu or thời gian giữa hai lần quẹo góc vuông > 8s
  // tránh trường hơp nhận nhầm góc vuông ở các trường hợp khác
    
    if(bias == 5) { //trượt khỏi góc vuông thì lùi lại
      myservo.write(centerDegree+mySensorPID.Calcu(0));
      digitalWrite(IN[0], LOW);
      digitalWrite(IN[1], HIGH);
      analogWrite(motorPin, 180);
      delay(100);
    }

    if (rightAngle == 1)  { // xử lí góc vuông
      RAProcess();
      if (firstRA == 1)
        firstRA = 0;
      timeGet = millis();
    }
  }

  // xử lý các trường hợp k phải góc vuông
  if (abs(bias) <= 4) {
    myservo.write(centerDegree+mySensorPID.Calcu(bias));
    int speed = 0;
    
    switch(abs(bias)) { // tốc độ 4 cấp rẽ (trái phải gộp chung)
      case 0:
        speed = 100; // tốc độ đi thẳng
        break;
      case 1:
        speed = 90; // quẹo trái/phải cấp 1
        break;
      case 2:
        speed = 100;
        break;
      case 3:
        speed = 130;
        break;
      case 4:
        speed = 130; // quẹo trái/phải cấp 4
        break;

    }

    
    digitalWrite(IN[0], HIGH);
    digitalWrite(IN[1], LOW);
    analogWrite(motorPin, speed);

   
  }


    
  bias = prebias;


  delay(10);
}

void RAProcess()  { // xử lý góc vuông

    myservo.write(centerDegree+mySensorPID.Calcu(0)); //lùi thẳng
    digitalWrite(IN[0], LOW);
    digitalWrite(IN[1], HIGH);
    analogWrite(motorPin, 180);
    delay(400);

    myservo.write(centerDegree+mySensorPID.Calcu(-4)); // tiến trái
    digitalWrite(IN[0], HIGH);
    digitalWrite(IN[1], LOW);
    analogWrite(motorPin, 180);
    delay(800);


    myservo.write(centerDegree+mySensorPID.Calcu(4)); // lùi trái
    digitalWrite(IN[0], LOW);
    digitalWrite(IN[1], HIGH);
    if (firstRA == 1) // góc vuông đầu
      delay(700); 
    else delay(690); // góc vuông thứ hai

    myservo.write(centerDegree+mySensorPID.Calcu(-4)); //tiến trái
    digitalWrite(IN[0], HIGH);
    digitalWrite(IN[1], LOW);
    delay(650);


}