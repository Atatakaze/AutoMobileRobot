#include <PID_v1.h>
#include <ros.h>
#include <math.h>
#include <std_msgs/Int32.h>

//left motor
const byte encoder1pinA = 3;//A pin -> the interrupt pin 1
const byte encoder1pinB = 9;//B pin -> the digital pin 3
int in1 = 10; //The enabling of L298PDC motor driver board connection to the digital interface port 5
int in2 = 11; //The enabling of L298PDC motor driver board connection to the digital interface port 4
int ena = 5; 
//right motor
const byte encoder0pinA = 2;//A pin -> the interrupt pin 0
const byte encoder0pinB = 8;//B pin -> the digital pin 3
int in3 = 12; //The enabling of L298PDC motor driver board connection to the digital interface port 5
int in4 = 13; //The enabling of L298PDC motor driver board connection to the digital interface port 4
int enb = 6; 

byte encoder0PinALast;
byte encoder1PinALast;
double durationright, abs_durationright;//the number of the pulses
double durationleft, abs_durationleft;
boolean Directionright;//the rotation direction
boolean Directionleft;
boolean resultright;
boolean resultleft;

ros::NodeHandle node;

double val_outputright;//Power supplied to the motor PWM value.
double val_outputleft;
double Setpointright = 0;
double Setpointleft = 0;

double Kp=0.6, Ki=5, Kd=0;

int MODE;
int count = 0;
boolean motorENABLE = false;

PID rightPID(&abs_durationright, &val_outputright, &Setpointright, Kp, Ki, Kd, DIRECT);
PID leftPID(&abs_durationleft, &val_outputleft, &Setpointleft, Kp, Ki, Kd, DIRECT);

void num(const std_msgs::Int32& msg){
  count++;
  if(count == 1)
    MODE = msg.data;
  else if(count == 2)
    Setpointleft = msg.data;
  else
  {
    Setpointright = msg.data;
    count=0;
    //motorENABLE = true;
  }
}
ros::Subscriber<std_msgs::Int32> sub("/pwm",&num);


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
   node.initNode();
   node.subscribe(sub);
   rightPID.SetMode(AUTOMATIC);//PID is set to automatic mode
   rightPID.SetSampleTime(100);//Set PID sampling frequency is 100ms
   leftPID.SetMode(AUTOMATIC);//PID is set to automatic mode
   leftPID.SetSampleTime(100);//S
   EncoderInit();//Initialize the module

   Serial.print("INIT DONE");
}



void loop()
{     
      if(MODE == 1)
        advance();//Motor Forward
      if(MODE == 2)
        back();
      if(MODE == 0)
        Stop();
      abs_durationright = abs(durationright);
      resultright = rightPID.Compute();//PID conversion is complete and returns 1
      if(resultright)
        durationright = 0; //Count clear, wait for the next count
      
      abs_durationleft = abs(durationleft);
      resultleft = leftPID.Compute();//PID conversion is complete and returns 1
      if(resultleft)
        durationleft = 0; //Count clear, wait for the next count

      node.spinOnce();
}

void EncoderInit()
{
  Directionright = true;//default -> Forward
  pinMode(encoder0pinB, INPUT);
  attachInterrupt(0, wheelSpeedright, CHANGE);
  Directionleft = true;//default -> Forward
  pinMode(encoder1pinB, INPUT);
  attachInterrupt(1, wheelSpeedleft, CHANGE);
}
void wheelSpeedright()
{
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && Lstate == HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && Directionright)
      Directionright = false; //Reverse
    else if(val == HIGH && !Directionright)
      Directionright = true;  //Forward
  }
  encoder0PinALast = Lstate;
  if(!Directionright)  
    durationright++;
  else  
    durationright--;
}
void wheelSpeedleft()
{ 
  int Rstate = digitalRead(encoder1pinA);
  if((encoder1PinALast == LOW) && Rstate==HIGH)
  {
    int valR = digitalRead(encoder1pinB);
    if(valR == LOW && Directionleft)
      Directionleft = false; //Reverse
    else if(valR == HIGH && !Directionleft)
      Directionleft = true;  //Forward
  }
  encoder1PinALast = Rstate;
  if(!Directionleft)  
    durationleft++;
  else  
    durationleft--;
}

void advance()//Motor Forward
{
     digitalWrite(in1,HIGH);
     digitalWrite(in2,LOW);
     digitalWrite(in3,HIGH);
     digitalWrite(in4,LOW);
     //if(motorENABLE)
     //{
       analogWrite(ena,val_outputleft);
       analogWrite(enb,val_outputright);
     //}
     //motorENABLE = false;
}

void back()//Motor reverse
{
     digitalWrite(in1,LOW);
     digitalWrite(in2,HIGH);
     digitalWrite(in3,LOW);
     digitalWrite(in4,HIGH);
     //if(motorENABLE)
     //{
       analogWrite(ena, -1 * Setpointleft);
       analogWrite(enb, -1 * Setpointright);
     //}
     //motorENABLE = false;
}

void Stop()//Motor stops
{
     digitalWrite(in1,LOW);
     digitalWrite(in2,LOW);
     digitalWrite(in3,LOW);
     digitalWrite(in4,LOW);
}

