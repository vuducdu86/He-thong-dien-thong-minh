/*
 * Created by Lam Hong Duc
 * Date 20/11/2020)
 * Edited 13/12/2020 by Lam Hong Duc
 */

 #include <LiquidCrystal_I2C.h>   // add liquidcrystal library

 int leftSensor = 4;
 int middleSensor = 3;
 int rightSensor = 2;
 int ENA = 5;
 int in1 = 6;
 int in2 = 7;
 int in3 = 8;
 int in4 = 9;
 int ENB = 10;
 int leftSpeed = 150;
 int rightSpeed = 150;
 LiquidCrystal_I2C lcd(0x27,16,2);
 
 void moveForward()
  {
    Serial.println("Move Forward");
    lcd.setCursor(0,1);
    lcd.print("Chay Toi        ");
    analogWrite(ENA,leftSpeed);
    analogWrite(ENB,rightSpeed);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);  
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  }

  void moveBackward()
  {
    Serial.println("Move Backward");
    lcd.setCursor(0,1);
    lcd.print("Chay Lui        ");
    analogWrite(ENA,leftSpeed);
    analogWrite(ENB,rightSpeed);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);  
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); 
  }
  
  void turnRight()
  {
    Serial.println("Turn Right");
    lcd.setCursor(0,1);
    lcd.print("Queo Phai       ");
    analogWrite(ENA,leftSpeed);
    analogWrite(ENB,rightSpeed/2);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);  
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
   }
  
  void turnLeft()
  {
    Serial.println("Turn Left");
    lcd.setCursor(0,1);
    lcd.print("Queo Trai       ");
    analogWrite(ENA,leftSpeed/2);
    analogWrite(ENB,rightSpeed);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH); 
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  }
  
  void moveStop()
  {
    Serial.println("Move Stop");
    lcd.setCursor(0,1);
    lcd.print("Ngung           ");
    analogWrite(ENA,LOW);
    analogWrite(ENB,LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW); 
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
  }

 void setup()
 {
  Serial.begin(9600);
  pinMode(leftSensor,INPUT);
  pinMode(middleSensor,INPUT);
  pinMode(rightSensor,INPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("LineFollowerCar");
  moveStop();
 }

void loop()
{
  Serial.println("stateLeft stateMiddle  stateRight");
  Serial.print(digitalRead(leftSensor));
  Serial.print("         ");
  Serial.print(digitalRead(middleSensor));
  Serial.print("            ");
  Serial.println(digitalRead(rightSensor));
  // middle senssor on black
  if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 0))
  {
    moveForward();
  }
  // left sensor & middle sensor on black
  if((digitalRead(leftSensor) == 1) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 0))
  {
    turnLeft();
  }
  // left sensor on black
  if((digitalRead(leftSensor) == 1) && (digitalRead(middleSensor) == 0) && (digitalRead(rightSensor) == 0))
  {
    turnLeft();
  }
  // middle sensor & right sensor on black
  if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 1))
  {
    turnRight();
  }
  // right sensor on black
  if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 0) && (digitalRead(rightSensor) == 1))
  {
    turnRight();
  }
  // all 3 on white
  if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 0) && (digitalRead(rightSensor) == 0))
  {
    moveStop();
  }
  // all 3 on black
  if((digitalRead(leftSensor) == 1) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 1))
  {
    moveForward();
  }
}
