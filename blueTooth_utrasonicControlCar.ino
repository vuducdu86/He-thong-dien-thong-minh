#include <LiquidCrystal_I2C.h>  //Thu Vien Cho LCD co I2C
#include <Servo.h>              //Thu vien Servo
#include <NewPing.h>            //Thu vien cho Cam bien sieu am

// Gán giá trị chân của mạch điều khiển động cơ L298N vào chân của Vi điều khiển
const int in1 = 4;             
const int in2 = 5;
const int in3 = 6;
const int in4 = 7;
const int buttonPin = 2; // Định nghĩa biến hằng buttonPin kết nối vào chân 2 trên Vi điều khiển
LiquidCrystal_I2C lcd(0x27,16,2);  //Dat ten lcd va Khai bao dia chi cua I2C So cot va hang

boolean buttonState = HIGH; // Khai báo biến trạng thái nút nhấn ở kiểu dữ liệu boolean chỉ nhận giá trị true

boolean debounceButton(boolean state) // Khai báo biến dội phím nút nhấn kiểu boolean
{
  boolean stateNow=digitalRead(buttonPin);
  if(state!=stateNow) //Nếu trạng thái khác với trạng thái hiện tại là TRUE thì thực hiện
  {
    delay(10); // trễ 10 mili giây
    stateNow=digitalRead(buttonPin); // Trạng thái hiện tại của nút nhấn chính là trạng thái của chân 2 của Adruino
    }
    return stateNow; // Trả về trạng thái hiện tại của nút nhấn
  }

int pressed = 0; // nút nhấn đã được nhấn kiểu interger
int val;         // value kiểu interger
int cm = 0;      // khoảng cách đến vật cản kiểu interger, giá trị ban đâu là 0
int duration=0;  // thời gian từ cảm biến đến vật cản
int iterations=5;// số lần thực hiện gói ping
int distance = 0;// khoảng cách từ cảm biến đến vật cản cả đi và về
int distanceRight = 0; // khoảng cách phải từ cảm biến đến vật cản
int distanceLeft = 0;  // khoảng cách trái từ cảm biến đến vật cản

#define trigPin A0 //Chân A0 kết nối vào chân trig trên cảm biến siêu âm
#define echoPin A1 //Chân A1 kết nối vào chân echo trên cảm biến siêu âm
#define maxDistance 400 // Khoảng cách tối đa cảm biến đo được

NewPing sonar(trigPin, echoPin, maxDistance); //Khởi tạo cảm biến siêu âm
Servo servo; //Khởi tạo servo có tên là servo

void setup()
{
  pinMode(in1, OUTPUT);//chân in1 trên module điều khiển động cơ là output
  pinMode(in2, OUTPUT);//chân in2 trên module điều khiển động cơ là output
  pinMode(in3, OUTPUT);//chân in3 trên module điều khiển động cơ là output
  pinMode(in4, OUTPUT);//chân in4 trên module điều khiển động cơ là output
  pinMode(buttonPin,INPUT_PULLUP); //chân 2 trên Vi điều khiển ở mode input có điện trở kéo
  Serial.begin(9600); //Khởi tạo cổng serial
  lcd.init();// khởi tạo lcd
  lcd.backlight();//Chế độ màn hình tối
  
  servo.attach(9); //chân tín hiệu của servo kết nối vào chân 9 của Vi điều khiển
  
  servo.write(87); // Server quay 87 độ
  delay(2000); // Delay 2 giây
}

void loop()
{
//  Serial.println("Press button to select control");
//  lcd.setCursor(0,0);
//  lcd.print("Bam nut de chon");
  if(debounceButton(buttonState)== LOW && buttonState == HIGH) // Gọi hàm dội phím nút nhấn có giá trị 0 và trạng thái nút nhấn được nhấn thì
  {
    pressed++;    // Biến ĐÃ NHẤN tăng lên 1 giá trị
    buttonState=LOW;  //Trạng thái nút nhấn về 0
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
