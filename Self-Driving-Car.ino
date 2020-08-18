//www.elegoo.com

//Line Tracking IO define
#define LT_R !digitalRead(10)//right sensor
#define LT_M !digitalRead(4)//middle sensor
#define LT_L !digitalRead(2)//left sensor

#define ENB 5 //left or right wheels speed
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6 // left or right wheels speed

//keeps track how many times we cross stop line
int counter = 0;

//The turnCounter is used to bring the car back to the track
//when it loses it (precisely when the car run over a broken line)
int turnCounter = 0;

int carSpeed = 130;// You can play with this value.

//In some cases (zig-zag, brorken line) we need to memorize 
//to which side we need ot turn and keep turning 
//even if the sensor doesn't detect line anymore.
//This is the use of lft
bool lft = 0;


 void finish(){//checks if the car is at the finish line (three perpendicular lines) and manipulates counter
     if(LT_L&&LT_M &&LT_R){//if all sensors are off -  we are on the finish line or out of the track
      forward();
        counter++;
        Serial.println( "Counter is:");
        Serial.println(counter,DEC);
    }
     if(counter >=3 ){
        stop();
     }
  }
//This method will kick in when the line become brocken.
//Once activated the car will remain under its control even if the line is not broken anymore.
//This regime is NOT suitable for other configurations of the line and you may want to change it.
//This method works so because it was initialy writen for a specific competition.
  void brokenLine(){
    if(turnCounter >80){
      back();
       while(!LT_M){
          if(lft){
            right();
          }else{
            left();
          }
        }
        carSpeed = 160;
      while(counter<3){
        if(LT_L){
         leftS();
        }
        else if(LT_R){
          rightS();
        }
        while(LT_M&& counter<3){
        forward();
        finish();
        }
      }
      while(true){
        stop;
      }
    }
  }

  void noTrack(){
    if(!LT_M){ //if it is beyound the track then reverse
      if(!LT_R){
        if(!LT_L){
          back();
          Serial.println("REVERSE");
          while(!(LT_M || LT_L || LT_R));
        }
      }
    }
  }

  void forward(){
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("go forward!");
  }
  
  void back(){
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("go back!");
  }
  
  void left(){
    if(carSpeed!=250){//reduce the speed if we are in sharp turn
    carSpeed = 250;
    }
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);//left wheels backwards
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);//right wheels on
    Serial.println("go left!");
  }

   void leftS(){// steer without other side wheels to move backwards
//  if(carSpeed!=250){//increase the speed. Its not a sharp turn
//  carSpeed = 250;
//  }
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go leftSpeed!");
}
  
  
  
  void right(){
   if(carSpeed!=250){//reduce the speed if we are in sharp turn
    carSpeed = 250;
    }
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);//right wheels on
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);//left wheels backward
    digitalWrite(IN4, LOW); 
    Serial.println("go right!");
  } 

   void rightS(){// steer without other side wheels to move backwards
//   if(carSpeed!=250){//increase the speed. Its not a sharp turn
//  carSpeed = 250;
//  }
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
  Serial.println("go rightSpeed!");
}
  
  
  
  void stop(){
     digitalWrite(ENA, LOW);
     digitalWrite(ENB, LOW);
     Serial.println("Stop!");
  } 
  
  void setup(){
    Serial.begin(9600);
  }
  
  void loop() {
  
    while(LT_M){
     // finish();
     //reset counter once the middle sensor is activated
       if(turnCounter>0){
        turnCounter = 0;
      }
     // noTrack();
      if(LT_L){
        lft = true; // if left sensor detects lft = true
      }else if(LT_R){
        lft = false;//if right sesnor detects lft = false
      }
      forward();
      
    }

   //we need ot stop from time to time
   //as this improves the reactions
   stop();
   
   if(lft && counter<3){
    left();
    if(counter >0){
      counter = 0;
    }
    turnCounter++;
   }
   else if(!lft && counter<3){
    right();
    if(counter>0){
      counter = 0;
    }
    turnCounter++;
   }
   stop();
   brokenLine();
   carSpeed = 130;


  }

    
