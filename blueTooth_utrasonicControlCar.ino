#include <LiquidCrystal_I2C.h>  //Thu Vien Cho LCD co I2C
#include <Servo.h>              //Thu vien Servo
#include <NewPing.h>            //Thu vien cho Ultrasonic Sensor


const int in1 = 4;
const int in2 = 5;
const int in3 = 6;
const int in4 = 7;
const int buttonPin = 2;
LiquidCrystal_I2C lcd(0x27,16,2);    //Dat ten lcd va Khai bao dia chi cua I2C So cot va hang

boolean buttonState = HIGH;

boolean debounceButton(boolean state)
{
  boolean stateNow=digitalRead(buttonPin);
  if(state!=stateNow)
  {
    delay(10);
    stateNow=digitalRead(buttonPin);
    }
    return stateNow;
  }

int pressed = 0;
int val;
int cm = 0;
int duration=0;
int iterations=5;
int distance = 0;
int distanceRight = 0;
int distanceLeft = 0;

#define trigPin A0 //analog input 1
#define echoPin A1 //analog input 2
#define maxDistance 400

NewPing sonar(trigPin, echoPin, maxDistance); //sensor function
Servo servo; //our servo name is servo

void setup()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  servo.attach(9); //our servo pin
  
  servo.write(87);
  delay(2000);
}

void loop()
{
//  Serial.println("Press button to select control");
//  lcd.setCursor(0,0);
//  lcd.print("Bam nut de chon");
  if(debounceButton(buttonState)== LOW && buttonState == HIGH)
  {
    pressed++;    
    buttonState=LOW;
    }
    else if(debounceButton(buttonState) == HIGH && buttonState == LOW)
    {
      buttonState = HIGH;
      }
  if(pressed==0){
    Serial.println("Press button to select control");
    lcd.setCursor(0,0);
    lcd.print("Bam nut de chon");
  }
  if(pressed==1)
  {
    Serial.println("Obstacle Advoiding Robot");
    lcd.setCursor(0,0);
    lcd.print("Xe Tranh Vat Can");
    obstacleAdvoiding();
    }
  if(pressed==2)
  {
    Serial.println("Bluetooth Control Car");
    lcd.setCursor(0,0);
    lcd.print("Xe Bluetooth    ");
    bluetoothCar();
  }
   if(pressed>=3)
  {
    pressed=0;
    lcd.clear();
    }
 }

void obstacleAdvoiding()
{
  distance = readPing();
  Serial.print("Distance Ahead is: ");
  Serial.println(distance);
  if (distance <= 20)
  {
    moveStop();
    delay(50);
    moveBackward();
    delay(400);
    moveStop();
    delay(50);
    distanceRight = lookRight();
    Serial.print("Distance Right is: ");
    Serial.print(distanceRight);
    Serial.print("/ ");
    delay(300);
    distanceLeft = lookLeft();
    Serial.print("Distance Left is: ");
    Serial.println(distanceLeft);
    Serial.println("");
    delay(300);

    if (distanceRight >= distanceLeft)
    {
      turnRight();
      delay(700);
      moveStop();
    }
    else
    {
      turnLeft();
      delay(760); 
      moveStop();
    }
  }
  else
  {
    moveForward();
  }
}

void bluetoothCar()
{
  while (Serial.available() > 0)
  {
    val = Serial.read();
  }
  if( val == 'F') //Forward
    {
      moveForward();  
    }
    else if(val == 'B') //Backward
    {
      moveBackward();
    }  
    else if(val == 'L') //Left
    {
      turnLeft();
    }
    else if(val == 'R') //Right
    {
      turnRight();
    }    
    else if(val == 'S') //Stop
    {
      moveStop();
    }
    else if(val == 'I') //Forward Right
    {
      forwardRight();
    }
    else if(val == 'J') //Backward Right
    {
      backwardRight();
    }
    else if(val == 'G') //Forward Left
    {
      forwardLeft();
    }
    else if(val == 'H') //Backward Left
    {
      backwardLeft();
    }
}

int readPing()
{
  delay(70);
  duration = sonar.ping_median(iterations);
  cm=(duration/2)*0.0343;
  if(cm>=400)
  {
    cm=400;
  }
  if(cm<=2)
  {
    cm=400;
  }
  return cm;
}

int lookRight()
{  
  servo.write(9);
  delay(500);
  distance = readPing();
  delay(100);
  return distance;
  delay(100);
  } 

int lookLeft()
{
  servo.write(179);
  delay(500);
  distance = readPing();
  delay(100);
  servo.write(87);
  return distance;
  delay(100);
  }

void moveForward()
{
  Serial.println("Move Forward");
  lcd.setCursor(0,1);
  lcd.print("Chay Toi        ");
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
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }

void turnLeft()
{
  Serial.println("Turn Left");
  lcd.setCursor(0,1);
  lcd.print("Queo Trai       ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
  }

void moveStop()
{
  Serial.println("Move Stop");
  lcd.setCursor(0,1);
  lcd.print("Ngung           ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }

void forwardRight()
{
  Serial.println("Forward Right");
  lcd.setCursor(0,1);
  lcd.print("Toi Phai        ");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }

void forwardLeft()
{
  Serial.println("Forward Left");
  lcd.setCursor(0,1);
  lcd.print("Toi Trai        ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  }

void backwardRight()
{
  Serial.println("Backward Right");
  lcd.setCursor(0,1);
  lcd.print("Lui Phai        ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }
  
void backwardLeft()
{
  Serial.println("Backward Left");
  lcd.setCursor(0,1);
  lcd.print("Lui Trai        ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
  }
