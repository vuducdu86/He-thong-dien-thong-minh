#include <Servo.h>              //Thu vien Servo
#include <NewPing.h>            //Thu vien cho Cam bien sieu am

// Gán giá trị chân của Sensor hong ngoai vào chân của Vi điều khiển
const int leftSensor = 4;
const int middleSensor = 3;
const int rightSensor = 2;
// Gán giá trị chân của mạch điều khiển động cơ L298N vào chân của Vi điều khiển
const int in1 = 5;             
const int in2 = 6;
const int in3 = 7;
const int in4 = 8;
const int buttonPin = 12; // Định nghĩa biến hằng buttonPin kết nối vào chân 12 trên Vi điều khiển
const int ENA = 10;
const int ENB = 11;
const int leftSpeed = 255;
const int rightSpeed = 255;

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
int val;         // giá trị nhận được từ điện thoại điều khiển bẳng bluetooth kiểu interger
int cm = 0;      // khoảng cách đến vật cản theo đơn vị cm
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
  pinMode(leftSensor,INPUT);
  pinMode(middleSensor,INPUT);
  pinMode(rightSensor,INPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(in1, OUTPUT);//chân in1 trên module điều khiển động cơ trái tiến
  pinMode(in2, OUTPUT);//chân in2 trên module điều khiển động cơ trái lùi
  pinMode(in3, OUTPUT);//chân in3 trên module điều khiển động cơ phải tiến
  pinMode(in4, OUTPUT);//chân in4 trên module điều khiển động cơ phải lùi
  pinMode(buttonPin,INPUT_PULLUP); //chân 2 trên Vi điều khiển ở mode input có điện trở kéo
  Serial.begin(9600); //Khởi tạo cổng serial
  servo.attach(9); //chân tín hiệu của servo kết nối vào chân 9 của Vi điều khiển
  servo.write(87); // Server quay 87 độ
  delay(2000); // Delay 2 giây
  moveStop();
}

void loop()// Chương trình lặp chính
{
//  Serial.println("Press button to select control");
  if(debounceButton(buttonState)== LOW && buttonState == HIGH) // Gọi hàm dội phím nút nhấn có giá trị 0 và trạng thái nút nhấn được nhấn thì
  {
    pressed++;    // Biến ĐÃ NHẤN tăng lên 1 giá trị
    buttonState=LOW;  //Trạng thái nút nhấn về 0
    }
    else if(debounceButton(buttonState) == HIGH && buttonState == LOW)//Nếu ngược lại thì trạng thái nút nhấn là 1
    {
      buttonState = HIGH;
      }
  if(pressed==0){                //Nếu chưa nhấn nút nhấn thì hiển thị Bấm nút để chọn
    Serial.println("Press button to select control");
   
    
  }
  if(pressed==1)                // Nếu nhấn lần 1 thì hiển thị Chương trình Xe tránh vật cản
  {
    Serial.println("Obstacle Advoiding Robot");
    
    
    obstacleAdvoiding();
    }
  if(pressed==2)                // Nếu nhấn lần 2 thì hiển thị chạy chương trình xe điều khiển bằng bluetooth
  {
    Serial.println("Bluetooth Control Car");
    bluetoothCar();
  }
  if(pressed==3)               // Nếu nhấn lần 3 chay chuong trinh lineFollowCar
  {
    Serial.println("Line Follow Car");
    lineFollowCar();   
  }
   if(pressed>=4)               // Nếu nhấn lần 4 thi xóa LCD, trở về trạng thái dừng
  {
    pressed=0;
    
    }
 }

void obstacleAdvoiding()     //Chương trình xe tự tránh vật cản bằng cảm biến siêu âm
{
  distance = readPing(); // Gán biến khoảng cách đến vật cản bằng cách đọc giá trị cảm biến
  Serial.print("Distance Ahead is: ");  // In giá trị khoảng cách ra serial monitor
  Serial.println(distance);
  if (distance <= 20)  // Nếu khoảng cách đến vật cản <20 cm
  {
    moveStop(); // Dừng lại
    delay(50);  
    moveBackward(); // Chạy chương trình con "Lùi lại"
    delay(400);  
    moveStop();  // Dừng lại
    delay(50); 
    distanceRight = lookRight(); // Khoảng cách phải bằng đọc giá trị chương trình con "Nhìn Phải"
    Serial.print("Distance Right is: ");  // In ra giá trị Khoảng cách bên phải là
    Serial.print(distanceRight);
    Serial.print("/ ");
    delay(300);
    distanceLeft = lookLeft();  // Khoảng cách trái bằng đọc giá trị chương trình con "Nhìn Trái"
    Serial.print("Distance Left is: ");  // In ra giá trị Khoảng cách bên trái là
    Serial.println(distanceLeft);
    Serial.println("");
    delay(300);

    if (distanceRight >= distanceLeft)  // Nếu khoảng cách Phải > Khoảng cách trái thì Rẽ Phải, rồi Dừng lại, không xoay vòng
    {
      turnRight();
      delay(700);
      moveStop();
    }
    else                               // Nếu Khoảng cách Phải < Khoản cách trái thì Rẽ trái rồi Dừng lại, không xoay vòng
    {
      turnLeft();
      delay(760); 
      moveStop();
    }
  }
  else
  {
    moveForward();           // Nếu khoảng cách đến vật cản > 20 cm thì đi Thẳng
  }
}

void bluetoothCar()  // Chương trình điều khiển bằng Bluetooth
{
  while (Serial.available() > 0) // Vòng lặp điều kiện trước: Nếu giá trị trên chân nối tiếp kết nối vào module bluetooth khác không
  {
    val = Serial.read();  // Biến đọc giá trị điều khiển bằng app
  }
  if( val == 'F') //Nếu nhấn F thì đi thẳng 
    {
      moveForward();  
    }
    else if(val == 'B') //Nếu nhấn B thì đi lùi
    {
      moveBackward();
    }  
    else if(val == 'L') //Nếu nhấn L thì rẽ trái
    {
      turnLeft();
    }
    else if(val == 'R') //nếu nhấn R thì rẽ phải
    {
      turnRight();
    }    
    else if(val == 'S') //Nếu nhấn S thì Dừng
    {
      moveStop();
    }
    else if(val == 'I') //Nếu nhấn I thì tiến phải
    {
      forwardRight();
    }
    else if(val == 'J') //Nếu nhấn J thì lùi Phải
    {
      backwardRight();
    }
    else if(val == 'G') //Nếu nhấn G thì Tiến trái
    {
      forwardLeft();
    }
    else if(val == 'H') //Nếu nhấn H thì Lùi trái
    {
      backwardLeft();
    }
}

//Chuong trinh con chạy dò line

void lineFollowCar()
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
// Chương trình con đọc giá trị cảm biến siêu âm
int readPing()
{
  delay(70);
  duration = sonar.ping_median(iterations);  // Đo khoảng thời gian bằng cách ping 5 lần
  cm=(duration/2)*0.0343;// khoảng cách đến vật cản = Thời gian * vận tốc âm thanh theo đơn vị cm/micro giây
  if(cm>=400)  // Nếu khoảng cách đến vật cản > 400 cm thì giá trị khoảng cách là 400cm vì giá trị tối đa mà cảm biến hoạt động được
  {
    cm=400;
  }
  if(cm<=2)
  {
    cm=400;   //Nếu khoảng cách đến vật cản < 2 cm thì giá trị khoảng cách là 2cm vì giá trị tối thiểu mà cảm biến hoạt động được
  }
  return cm; // Trả về giá trị khoảng cách
}
// Chương trình con Nhìn Phải
int lookRight()
{  
  servo.write(9); // Chân server kết nối đến cổng 9 của aduino
  delay(500);
  distance = readPing(); // Đọc giá trị cảm biến siêu âm
  delay(100);
  return distance; //trả về giá trị khoảng cách
  delay(100);
  } 

int lookLeft()  //Chương trình con " nhìn trái
{
  servo.write(179);  // Quay server 179 độ
  delay(500);
  distance = readPing();
  delay(100);
  servo.write(87);  // Quay server 87 độ
  return distance;
  delay(100);
  }

void moveForward()  //Chương trình con đi thẳng
{
  Serial.println("Move Forward");
  analogWrite(ENA,leftSpeed);
  analogWrite(ENB,rightSpeed);
  digitalWrite(in1, HIGH);       
  digitalWrite(in2, LOW);        
  digitalWrite(in3, HIGH);       
  digitalWrite(in4, LOW);        
 }

void moveBackward()   // Chương trình con đi lùi
{
  Serial.println("Move Backward");
  analogWrite(ENA,leftSpeed);
  analogWrite(ENB,rightSpeed);  
  digitalWrite(in1, LOW);       
  digitalWrite(in2, HIGH);     
  digitalWrite(in3, LOW);       
  digitalWrite(in4, HIGH);      
 }

void turnRight()    // Chương trình con rẽ phải
{
  Serial.println("Turn Right"); 
  analogWrite(ENA,leftSpeed);
  analogWrite(ENB,rightSpeed/2);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }

void turnLeft()       // Chương trình con rẽ trái
{
  Serial.println("Turn Left"); 
  analogWrite(ENA,leftSpeed/2);
  analogWrite(ENB,rightSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
  }

void moveStop()       //Chương trình con Dừng
{
  Serial.println("Move Stop");
  analogWrite(ENA,LOW);
  analogWrite(ENB,LOW); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }

void forwardRight()     // Chương trình con Tiến Phải
{
  Serial.println("Forward Right"); 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }

void forwardLeft()       // Chương trình con Tiến trái
{
  Serial.println("Forward Left");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  }

void backwardRight()      //Chương trình con Lùi Phải
{
  Serial.println("Backward Right"); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }
  
void backwardLeft()       //Chương trình con Lùi trái
{
  Serial.println("Backward Left"); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
  }
