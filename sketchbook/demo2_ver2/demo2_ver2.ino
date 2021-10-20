#include <PID_v1.h>
#include <ros.h>
#include <math.h>
#include <std_msgs/Int32.h>

//#include <std_msgs/Float32MultiArray.h>
const byte encoder0pinA = 3;//A pin -> the interrupt pin 0
const byte encoder0pinB = 9;//B pin -> the digital pin 3
int in1 = 10; //The enabling of L298PDC motor driver board connection to the digital interface port 5
int in2 = 11; //The enabling of L298PDC motor driver board connection to the digital interface port 4
int ena = 5; 
const byte encoder1pinA = 2;//A pin -> the interrupt pin 0
const byte encoder1pinB = 8;//B pin -> the digital pin 3
int in3 = 12; //The enabling of L298PDC motor driver board connection to the digital interface port 5
int in4 = 13; //The enabling of L298PDC motor driver board connection to the digital interface port 4
int enb = 6; 

byte encoder0PinALast;
byte encoder1PinALast;
double durationright,abs_durationright;//the number of the pulses
double durationleft,abs_durationleft;
boolean Directionright;//the rotation direction
boolean Directionleft;
boolean resultright;
boolean resultleft;

ros::NodeHandle nh;
//int count=1;
double val_outputright;//Power supplied to the motor PWM value.
double val_outputleft;
double Setpointright = 0;
double Setpointleft = 0;
double Kp=0.6, Ki=5, Kd=0;

PID rightPID(&abs_durationright, &val_outputright, &Setpointright, Kp, Ki, Kd, DIRECT);
PID leftPID(&abs_durationleft, &val_outputleft, &Setpointleft, Kp, Ki, Kd, DIRECT);


void num1(const std_msgs::Int32& msg){
    Setpointleft = msg.data;
}

void num2(const std_msgs::Int32& msg){
    Setpointright = msg.data;
}

ros::Subscriber<std_msgs::Int32> sub1("/pwmL", &num1);
ros::Subscriber<std_msgs::Int32> sub2("/pwmR", &num2);

void setup()
{
   Serial.begin(57600);//Initialize the serial port
   pinMode(in1, OUTPUT);   //L298P Control port settings DC motor driver board for the output mode
   pinMode(in2, OUTPUT);
   pinMode(ena, OUTPUT);
   pinMode(in3, OUTPUT);   //L298P Control port settings DC motor driver board for the output mode
   pinMode(in4, OUTPUT);
   pinMode(enb, OUTPUT);
   pinMode(encoder0pinA, INPUT);
   pinMode(encoder0pinB, INPUT);
   pinMode(encoder1pinA, INPUT);
   pinMode(encoder1pinB, INPUT);
   nh.initNode();
   
   nh.subscribe(sub1);
   nh.subscribe(sub2);
   
   rightPID.SetMode(AUTOMATIC);//PID is set to automatic mode
   rightPID.SetSampleTime(100);//Set PID sampling frequency is 100ms
   leftPID.SetMode(AUTOMATIC);//PID is set to automatic mode
   leftPID.SetSampleTime(100);//S
   EncoderInit();//Initialize the module
   
   Serial.print("INIT DONE");
}



void loop()
{     
      abs_durationright=abs(durationright);
      resultright=rightPID.Compute();//PID conversion is complete and returns 1
      if(resultright)
        durationright = 0; //Count clear, wait for the next count
      
      abs_durationleft=abs(durationleft);
      resultleft=leftPID.Compute();//PID conversion is complete and returns 1
      if(resultleft)
        durationleft = 0; //Count clear, wait for the next count

      nh.spinOnce();
      nh.spinOnce();
      
      run();
       
}



void EncoderInit()
{
  Directionright = true;//default -> Forward
  pinMode(encoder0pinB,INPUT);
  attachInterrupt(0, wheelSpeedright, CHANGE);
  Directionleft = true;//default -> Forward
  pinMode(encoder1pinB,INPUT);
  attachInterrupt(1, wheelSpeedleft, CHANGE);
}
void wheelSpeedright()
{
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && Directionright)
    {
      Directionright = false; //Reverse
    }
    else if(val == HIGH && !Directionright)
    {
      Directionright = true;  //Forward
    }
  }
  encoder0PinALast = Lstate;
  if(!Directionright)  durationright++;
  else  durationright--;
}
void wheelSpeedleft()
{ 
  int Rstate = digitalRead(encoder1pinA);
  if((encoder1PinALast == LOW) && Rstate==HIGH)
  {
    int valR = digitalRead(encoder1pinB);
    if(valR == LOW && Directionleft)
    {
      Directionleft = false; //Reverse
    }
    else if(valR == HIGH && !Directionleft)
    {
      Directionleft = true;  //Forward
    }
  }
  encoder1PinALast = Rstate;
  if(!Directionleft)  durationleft++;
  else  durationleft--;
}

// run
void run()
{
  if(Setpointleft == 0)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  if(Setpointright == 0)
  {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  if(Setpointleft > 0)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, val_outputleft);
  }
  if(Setpointright > 0)
  {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, val_outputright);
  }
  if(Setpointleft < 0)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena, val_outputleft);
  }
  if(Setpointright < 0)
  {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enb, val_outputright);
  }
}


