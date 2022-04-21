//WTFB
#define dirPin1 2     //stepper1 direction_pin
#define stepPin1 3    //stepper1 step_pin
#define dirPin2 4   //stepper2 direction_pin
#define stepPin2 5   //stepper2 step_pin
#define dirPin3 6    //stepper3 direction_pin
#define stepPin3 7   //stepper3 step_pin
#define dirPin4 9    //stepper4 direction_pin
#define stepPin4 10   //stepper4 step_pin
#define dirPin5 11   //stepper5 direction_pin
#define stepPin5 12  //stepper5 step_pin

#define HALL_SENSOR 22//hall sensor pin
#define HALL_SENSORA 23 //hall sensor pin 
#define HALL_SENSORB 24 //hall sensor pin 
#define HALL_SENSORC 25 //hall sensor pin 
#define HALL_SENSORD 26 //hall sensor pin 


//#define stepsPerRevolution 10000 //steps per revolution for each motor

#define X_DIR_PIN          2
#define X_STEP_PIN         3
#define X_ENABLE_PIN       38

#define Y_DIR_PIN          4
#define Y_STEP_PIN         5
#define Y_ENABLE_PIN       56

#define Z_DIR_PIN          6
#define Z_STEP_PIN         7
#define Z_ENABLE_PIN       62

#define A_DIR_PIN          9
#define A_STEP_PIN         10
#define A_ENABLE_PIN       24

#define B_DIR_PIN          11
#define B_STEP_PIN         12
#define B_ENABLE_PIN       30
float negativeAng;

String data;   // string variable to store incoming serial data
bool start_flag=false; // bool variable to store the start and pause status
bool start_calibrate=false;

// OHMMETER START
bool temp11 = true;
const int NUM=5; 
int sensorPin;
typedef struct
{
  float Vin;
  float Vout;
  float Rref;
  float R;
  int sensorValue;
  bool flag; // false means all is well true means wire is broken.
  int z;
  
}Station;
Station s[NUM];


// OHMMETER END

int ctrz1=0;
int ctrz2=0;
int ctrz3=0;
int ctrz4=0;
int ctrz5=0;

float motor_speed=0;
float motor1_speed=0;
float motor2_speed=0;
float motor3_speed=0;
float motor4_speed=0;
float motor5_speed=0;

unsigned long myTime;

//float baseMotorSpeed1 = 87;
float stepperMotorSpeed1=0;
float stepperMotorSpeed2=0;
float stepperMotorSpeed3=0;
float stepperMotorSpeed4=0;
float stepperMotorSpeed5=0;

float angle_diffz;
float angle_diff1;
float speed_diff1;
float speed_fast1;

int hallstate =0;
int hallstateA =0;
int hallstateB =0;
int hallstateC =0;
int hallstateD =0;

float start_angle=0;
float start_angle1=0;
float start_angle2=0; 
float start_angle3=0; 
float start_angle4=0; 
float start_angle5=0; 
 
float stop_angle=0;
float stop_angle1=0;
float stop_angle2=0;
float stop_angle3=0;
float stop_angle4=0;
float stop_angle5=0;

float temp = angle_diff1;
int val = (int) temp;

int cycle_count=0;
int cycle_count1=0;
int cycle_count2=0;
int cycle_count3=0;
int cycle_count4=0;
int cycle_count5=0;

int counter=0;
int counter1=0;
int counter2=0;
int counter3=0;
int counter4=0;
int counter5=0;

String station1_status;
String station2_status;
String station3_status;
String station4_status;
String station5_status;

bool s1=false;
bool s2=false;
bool s3=false;
bool s4=false;
bool s5=false;

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);

struct stepperInfo {
  // externally defined parameters
  float acceleration;
  volatile unsigned int minStepInterval;   // ie. max speed, smaller is faster
  void (*dirFunc)(int);
  void (*stepFunc)();

  // derived parameters
  unsigned int c0;                // step interval for first step, determines acceleration
  long stepPosition;              // current position of stepper (total of all movements taken so far)

  // per movement variables (only changed once per movement)
  volatile int dir;                        // current direction of movement, used to keep track of position
  volatile unsigned int totalSteps;        // number of steps requested for current movement
  volatile bool movementDone = false;      // true if the current movement has been completed (used by main program to wait for completion)
  volatile unsigned int rampUpStepCount;   // number of steps taken to reach either max speed, or half-way to the goal (will be zero until this number is known)

  // per iteration variables (potentially changed every interrupt)
  volatile unsigned int n;                 // index in acceleration curve, used to calculate next interval
  volatile float d;                        // current interval length
  volatile unsigned long di;               // above variable truncated
  volatile unsigned int stepCount;         // number of steps completed in current movement
};

void xStep() {
  //X_STEP_HIGH
  digitalWrite(X_STEP_PIN,HIGH);
 // X_STEP_LOW
  digitalWrite(X_STEP_PIN,LOW);
}
void xDir(int dir) {
  digitalWrite(X_DIR_PIN, dir);
}

void yStep() {
  //Y_STEP_HIGH
  //Y_STEP_LOW
  digitalWrite(Y_STEP_PIN,HIGH);
  digitalWrite(Y_STEP_PIN,LOW);
}
void yDir(int dir) {
  digitalWrite(Y_DIR_PIN, dir);
}

void zStep() {
  digitalWrite(Z_STEP_PIN,HIGH);
  digitalWrite(Z_STEP_PIN,LOW);
}
void zDir(int dir) {
  digitalWrite(Z_DIR_PIN, dir);
}
void aStep() {
  digitalWrite(A_STEP_PIN,HIGH);
  digitalWrite(A_STEP_PIN,LOW);
}
void aDir(int dir) {
  digitalWrite(A_DIR_PIN, dir);
}
void bStep() {
  digitalWrite(B_STEP_PIN,HIGH);
  digitalWrite(B_STEP_PIN,LOW);
}

void bDir(int dir) {
  digitalWrite(B_DIR_PIN, dir);
}

void resetStepperInfo( stepperInfo& si ) {
  si.n = 0;
  si.d = 0;
  si.di = 0;
  si.stepCount = 0;
  si.rampUpStepCount = 0;
  si.totalSteps = 0;
  si.stepPosition = 0;
  si.movementDone = false;
}

#define NUM_STEPPERS 5

volatile stepperInfo steppers[NUM_STEPPERS];
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600); ////////////////////////////////////USED TO BE Serial1.begin(9600)/////////////////
  // step and pins 1-5 are from the old code
 
  pinMode(stepPin1, INPUT);
  pinMode(dirPin1, INPUT);
  pinMode(stepPin2, INPUT);
  pinMode(dirPin2, INPUT);
  pinMode(stepPin3, INPUT);
  pinMode(dirPin3, INPUT);
  pinMode(stepPin4, INPUT);
  pinMode(dirPin4, INPUT);
  pinMode(stepPin5, INPUT);
  pinMode(dirPin5, INPUT);

/*
  pinMode(X_STEP_PIN,   OUTPUT);
  pinMode(X_DIR_PIN,    OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN,   OUTPUT);
  pinMode(Y_DIR_PIN,    OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
*/
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);
  digitalWrite(A_ENABLE_PIN, LOW);
  digitalWrite(B_ENABLE_PIN, LOW);
  
  for(int i=0; i<NUM; i++)
  {
    s[i].Vin=5;
    s[i].Vout=0;
    s[i].Rref=500;
    s[i].R=0;
    s[i].sensorValue=0;
    s[i].flag=false;
    s[i].z=1;
    Serial.print(i);
    
  } 
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 1000;                             // compare value
  TCCR1B |= (1 << WGM12);                   // CTC mode
  TCCR1B |= ((1 << CS11) | (1 << CS10));    // 64 prescaler
  interrupts();

  steppers[0].dirFunc = xDir;
  steppers[0].stepFunc = xStep;
  //steppers[0].acceleration = 5000; //5000
  steppers[0].minStepInterval = 80;

  steppers[1].dirFunc = yDir;
  steppers[1].stepFunc = yStep;
  //steppers[1].acceleration = 5000; //5000
  steppers[1].minStepInterval = 80;

  steppers[2].dirFunc = zDir;
  steppers[2].stepFunc = zStep;
  //steppers[2].acceleration = 1000;
  steppers[2].minStepInterval = 80;

  steppers[3].dirFunc = aDir;
  steppers[3].stepFunc = aStep;
  //steppers[2].acceleration = 1000;
  steppers[3].minStepInterval = 80;

  steppers[4].dirFunc = bDir;
  steppers[4].stepFunc = bStep;
  //steppers[2].acceleration = 1000;
  steppers[4].minStepInterval = 80;
}
    
void resetStepper(volatile stepperInfo& si) {
  si.c0 = si.acceleration;
  si.d = si.c0;
  si.di = si.d;
  si.stepCount = 0;
  si.n = 0;
  si.rampUpStepCount = 0;
  si.movementDone = false;
}

volatile byte remainingSteppersFlag = 0;

void prepareMovement(int whichMotor, int steps) {
  volatile stepperInfo& si = steppers[whichMotor];
  si.dirFunc( steps < 0 ? HIGH : LOW );
  si.dir = steps > 0 ? 1 : -1;
  si.totalSteps = abs(steps);
  resetStepper(si);
  remainingSteppersFlag |= (1 << whichMotor);
}

volatile byte nextStepperFlag = 0;

volatile int ind = 0;
volatile unsigned int intervals[100];

void setNextInterruptInterval() {

  bool movementComplete = true;

  unsigned int mind = 999999;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di < mind ) {
      mind = steppers[i].di;
    }
  }

  nextStepperFlag = 0;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ! steppers[i].movementDone )
      movementComplete = false;

    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di == mind )
      nextStepperFlag |= (1 << i);
  }

  if ( remainingSteppersFlag == 0 ) {
    OCR1A = 65500;
  }

  OCR1A = mind;
}

ISR(TIMER1_COMPA_vect)
{
  unsigned int tmpCtr = OCR1A;

  OCR1A = 65500;

  for (int i = 0; i < NUM_STEPPERS; i++) {

    if ( ! ((1 << i) & remainingSteppersFlag) )
      continue;

    if ( ! (nextStepperFlag & (1 << i)) ) {
      steppers[i].di -= tmpCtr;
      continue;
    }

    volatile stepperInfo& s = steppers[i];

    if ( s.stepCount < s.totalSteps ) {
      s.stepFunc();
      s.stepCount++;
      s.stepPosition += s.dir;
      if ( s.stepCount >= s.totalSteps ) {
        s.movementDone = true;
        remainingSteppersFlag &= ~(1 << i);
      }
    }

    if ( s.rampUpStepCount == 0 ) {
      s.n++;
      s.d = s.d - (2 * s.d) / (4 * s.n + 1);
      if ( s.d <= s.minStepInterval ) {
        s.d = s.minStepInterval;
        s.rampUpStepCount = s.stepCount;
      }
      if ( s.stepCount >= s.totalSteps / 2 ) {
        s.rampUpStepCount = s.stepCount;
      }
    }
    else if ( s.stepCount >= s.totalSteps - s.rampUpStepCount ) {
      s.d = (s.d * (4 * s.n + 1)) / (4 * s.n + 1 - 2);
      s.n--;
    }

    s.di = s.d; // integer
  }

  setNextInterruptInterval();

  TCNT1  = 0;
}

void runAndWait() {
  setNextInterruptInterval();
  //ohmmeter();
  while ( remainingSteppersFlag ); /// THIS may be the issue
}
void loop() 
{ 
serial_data();
if((start_flag==true))  //if start button is pressed and station1 button is pressed &&(station1_status=="ON")
 {
  //stepper_motor1(); // stepper1 motor function call
  Serial.print("test1");
  if(counter1>=cycle_count1||temp11==false)  // if counter1 is greater than entered count cycle then reset it.
  {
    motor1_speed=0;
    start_angle1=0;
    stop_angle1=0;
    cycle_count1=0;
    counter1=0;
    s[0].Vin=5;
    s[0].Vout=0;
    s[0].Rref=500;
    s[0].R=0;
    s[0].sensorValue=0;
    s[0].flag=false;
    s[0].z=1;
    //start_flag=false;
    //s[0].flag=false;
    s[1].Vin=5;
    s[1].Vout=0;
    s[1].Rref=500;
    s[1].R=0;
    s[1].sensorValue=0;
    s[1].flag=false;
    s[1].z=1;
    
    s[2].Vin=5;
    s[2].Vout=0;
    s[2].Rref=500;
    s[2].R=0;
    s[2].sensorValue=0;
    s[2].flag=false;
    s[2].z=1;

    s[3].Vin=5;
    s[3].Vout=0;
    s[3].Rref=500;
    s[3].R=0;
    s[3].sensorValue=0;
    s[3].flag=false;
    s[3].z=1;

    s[4].Vin=5;
    s[4].Vout=0;
    s[4].Rref=500;
    s[4].R=0;
    s[4].sensorValue=0;
    s[4].flag=false;
    s[4].z=1;
    
    start_flag=false;
    temp11= true;
    int ctrz1=0;
    int ctrz2=0;
    int ctrz3=0;
    int ctrz4=0;
    int ctrz5=0;
  }
   setBack();
   TIMER1_INTERRUPTS_ON
  Serial.println(angle_diff1);
  Serial.println(negativeAng);
  for(int i=0; i<cycle_count; i++)
  {
    ohmmeter();
    ohmmeterA();
    ohmmeterB();
    ohmmeterC();
    ohmmeterD();
    progress();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, angle_diff1 );
  runAndWait();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, negativeAng );
    counter1=counter1+1;
   runAndWait();
  //Serial.print("BRO");
  //Serial.println(i);
    //ohmmeterA();
    //ohmmeterC();
  //temp11 = ohmmeter(i);//calling ohmmeter
  
  }
  //goto label;
  //TIMER1_INTERRUPTS_OFF
  //while (true);
   start_flag=false;
      comeBack();

 // TIMER1_INTERRUPTS_OFF
 }

}

void setBack(){
  if ((station1_status == "ON") || (station2_status == "ON")||(station3_status == "ON")||(station4_status == "ON")||(station5_status == "ON")){
    if(station1_status == "ON")
    {
      pinMode(stepPin1, OUTPUT);
      pinMode(dirPin1, OUTPUT);
      digitalWrite(dirPin1, HIGH);
    }
    if(station2_status == "ON")
    {
      pinMode(stepPin2, OUTPUT);
      pinMode(dirPin2, OUTPUT);
      digitalWrite(dirPin2, HIGH);
    }
    if(station3_status == "ON")
    {
      pinMode(stepPin3, OUTPUT);
      pinMode(dirPin3, OUTPUT);
      digitalWrite(dirPin3, HIGH);
    }
    if(station4_status == "ON")
    {
      pinMode(stepPin4, OUTPUT);
      pinMode(dirPin4, OUTPUT);
      digitalWrite(dirPin4, HIGH);
    }
    if(station5_status == "ON")
    {
    pinMode(stepPin5, OUTPUT);
    pinMode(dirPin5, OUTPUT);
    digitalWrite(dirPin5, HIGH);
    }
  }
   
    for (int i = 0; i <  angle_diff1/2; i++)     // These four lines result in 1 step:
        {
          digitalWrite(stepPin1, HIGH);
          digitalWrite(stepPin2, HIGH);
          digitalWrite(stepPin3, HIGH);
          digitalWrite(stepPin4, HIGH);
          digitalWrite(stepPin5, HIGH);
          delayMicroseconds(1500);  // was 1500 both 
          digitalWrite(stepPin1, LOW);
          digitalWrite(stepPin2, LOW);
          digitalWrite(stepPin3, LOW);
          digitalWrite(stepPin4, LOW);
          digitalWrite(stepPin5, LOW);
          delayMicroseconds(1500);
       }  
    //Serial.println("DONE setback");
}
void comeBack(){
  if (station1_status == "ON"){
    pinMode(stepPin1, OUTPUT);
    pinMode(dirPin1, OUTPUT);
    //digitalWrite(dirPin1, LOW);
  }
  if (station2_status == "ON"){
    pinMode(stepPin2, OUTPUT);
    pinMode(dirPin2, OUTPUT);
    //digitalWrite(dirPin2, LOW);
  }
  if (station3_status == "ON"){
    pinMode(stepPin3, OUTPUT);
    pinMode(dirPin3, OUTPUT);
    //digitalWrite(dirPin3, LOW);
  }
  if (station4_status == "ON"){
    pinMode(stepPin4, OUTPUT);
    pinMode(dirPin4, OUTPUT);
    //digitalWrite(dirPin4, LOW);
  }
  if (station5_status == "ON"){
    pinMode(stepPin5, OUTPUT);
    pinMode(dirPin5, OUTPUT);
    //digitalWrite(dirPin5, LOW);
    
  }
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, LOW);
    digitalWrite(dirPin3, LOW);
    digitalWrite(dirPin4, LOW);
    digitalWrite(dirPin5, LOW);
    for (int i = 0; i <  angle_diff1/2; i++)     // These four lines result in 1 step:
        {
          digitalWrite(stepPin1, HIGH);
          digitalWrite(stepPin2, HIGH);
          digitalWrite(stepPin3, HIGH);
          digitalWrite(stepPin4, HIGH);
          digitalWrite(stepPin5, HIGH);
          delayMicroseconds(1500);
          digitalWrite(stepPin1, LOW);
          digitalWrite(stepPin2, LOW);
          digitalWrite(stepPin3, LOW);
          digitalWrite(stepPin4, LOW);
          digitalWrite(stepPin5, LOW);
          delayMicroseconds(1500);
       }  
    //Serial.println("DONE comeback");
}

///////////Calibration function///////////
void calibrate(){
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(dirPin4, OUTPUT);
  pinMode(stepPin5, OUTPUT);
  pinMode(dirPin5, OUTPUT);
  
  pinMode (HALL_SENSOR, INPUT);
  /*
  pinMode (HALL_SENSORA, INPUT);
  pinMode (HALL_SENSORB, INPUT);
  pinMode (HALL_SENSORC, INPUT);
  pinMode (HALL_SENSORD, INPUT);
*/

  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);
  digitalWrite(dirPin3, HIGH);
  digitalWrite(dirPin4, HIGH);
  digitalWrite(dirPin5, HIGH);
    
        for (int i = 0; i < 50; i++)     // These four lines result in 1 step:
        {
          digitalWrite(stepPin1, HIGH);
          digitalWrite(stepPin2, HIGH);
          digitalWrite(stepPin3, HIGH);
          digitalWrite(stepPin4, HIGH);
          digitalWrite(stepPin5, HIGH);
          delayMicroseconds(3000);
          digitalWrite(stepPin1, LOW);
          digitalWrite(stepPin2, LOW);
          digitalWrite(stepPin3, LOW);
          digitalWrite(stepPin4, LOW);
          digitalWrite(stepPin5, LOW);
          delayMicroseconds(3000);
       }
        hallstate = digitalRead(HALL_SENSOR);
        /*
        hallstateA=digitalRead(HALL_SENSORA);
        hallstateB=digitalRead(HALL_SENSORB);
        hallstateC=digitalRead(HALL_SENSORC);
        hallstateD=digitalRead(HALL_SENSORD);
        */
        Serial.println(hallstate);
        /*
        Serial.println(hallstateA);
        Serial.println(hallstateB);
        Serial.println(hallstateC);
        Serial.println(hallstateD);
*/
        while(hallstate == 1){
          hallstate=digitalRead(HALL_SENSOR);
          /*
          hallstateA=digitalRead(HALL_SENSORA);
          hallstateB=digitalRead(HALL_SENSORB);
          hallstateC=digitalRead(HALL_SENSORC);
          hallstateD=digitalRead(HALL_SENSORD);
          */
          digitalWrite(dirPin1, LOW);
          digitalWrite(dirPin2, LOW);
          digitalWrite(dirPin3, LOW);
          digitalWrite(dirPin4, LOW);
          digitalWrite(dirPin5, LOW);
         
          for (int i =0; i<5; i++){
          
            digitalWrite(stepPin1, HIGH);
            delayMicroseconds(3000);
            digitalWrite(stepPin2, HIGH);
            delayMicroseconds(3000);
            digitalWrite(stepPin3, HIGH);
            delayMicroseconds(3000);
            digitalWrite(stepPin4, HIGH);
            delayMicroseconds(3000);
            digitalWrite(stepPin5, HIGH);
            delayMicroseconds(3000);
            digitalWrite(stepPin1, LOW);
            delayMicroseconds(3000);
            digitalWrite(stepPin2, LOW);
            delayMicroseconds(3000);
            digitalWrite(stepPin3, LOW);
            delayMicroseconds(3000);
            digitalWrite(stepPin4, LOW);
            delayMicroseconds(3000);
            digitalWrite(stepPin5, LOW);
            delayMicroseconds(3000);
          }
          Serial.println(hallstate);
          /*
          Serial.println(hallstateA);
          Serial.println(hallstateB);
          Serial.println(hallstateC);
          Serial.println(hallstateD);
          */
          
        }
        if (hallstate == 0){
          Serial.println(hallstate);
          /*
          Serial.println(hallstateA);
          Serial.println(hallstateB);
          Serial.println(hallstateC);
          Serial.println(hallstateD);
          */
          }
        

}

//////////////////////////////////////////////
void serial_data()
{
 while(Serial1.available())  //wait until serial data is available
    {   
     if(Serial1.available() > 0) // if available data is greater than zero
       {
        data=Serial1.readString(); //Read serial data on Rx1,Tx1
        Serial.println(data);      // Print serial data
        if(data.startsWith("CALIBRATE"))
        {start_calibrate=true;
         calibrate();
        }

        else{ start_calibrate=false;}
        if(data.startsWith("START"))  
        {start_flag=true;}
        
       if(data.startsWith("PAUSE"))
        {start_flag=false;}

        if(data.startsWith("s1"))
        {
          s1=true;
          s2=false;
          s3=false;
          s4=false;
          s5=false;
        }

        if(data.startsWith("s2"))
        {
          s1=false;
          s2=true;
          s3=false;
          s4=false;
          s5=false;
        }

        if(data.startsWith("s3"))
        {
          s1=false;
          s2=false;
          s3=true;
          s4=false;
          s5=false;
        }

        if(data.startsWith("s4"))
        {
          s1=false;
          s2=false;
          s3=false;
          s4=true;
          s5=false;
        }

        if(data.startsWith("s5"))
        {
          s1=false;
          s2=false;
          s3=false;
          s4=false;
          s5=true;
        }
  
       if(data.startsWith("save"))
          {
        int start_angle_index = data.indexOf("start_angle");
        int stop_angle_index = data.indexOf("stop_angle");
        int cycle_count_index= data.indexOf("cycle_count");
        int slider_index= data.indexOf("slider");
        int station1_index=data.indexOf("station1=");
        int station2_index=data.indexOf("station2=");
        int station3_index=data.indexOf("station3=");
        int station4_index=data.indexOf("station4=");
        int station5_index=data.indexOf("station5=");
        
       String String_start_angle = data.substring(start_angle_index+11, stop_angle_index);
       start_angle=String_start_angle.toInt();
       Serial.print("start_angle=");
       Serial.println(start_angle);
       //////////////
       String String_stop_angle = data.substring(stop_angle_index+10,cycle_count_index);
       stop_angle=String_stop_angle.toInt();
       Serial.print("stop_angle=");
       Serial.println(stop_angle);
       //////////////
       String String_cycle_count=data.substring(cycle_count_index+11);
       cycle_count=String_cycle_count.toInt();
       Serial.print("cycle_count1=");
       Serial.println(cycle_count);
       /////////////
       String String_motor_speed=data.substring(slider_index+6);
       motor_speed=String_motor_speed.toInt();
       Serial.print("motor_speed=");
       Serial.println(motor_speed);
        /////////////
       String string_station1=data.substring(station1_index+9,station2_index);
       station1_status=string_station1;
       Serial.print("station1_status=");
       Serial.println(station1_status);
       if(station1_status=="ON")
       {
        pinMode(X_STEP_PIN,   OUTPUT);
        pinMode(X_DIR_PIN,    OUTPUT);
        pinMode(X_ENABLE_PIN, OUTPUT);
       }
       if(station1_status=="OFF")
       {
        pinMode(X_STEP_PIN,   INPUT);
        pinMode(X_DIR_PIN,    INPUT);
        pinMode(X_ENABLE_PIN, INPUT);
       }
       //////////////
       String string_station2=data.substring(station2_index+9,station3_index);
       station2_status=string_station2;
       Serial.print("station2_status=");
       Serial.println(station2_status);
       if(station2_status=="ON")
       {
         pinMode(Y_STEP_PIN,   OUTPUT);
         pinMode(Y_DIR_PIN,    OUTPUT);
         pinMode(Y_ENABLE_PIN, OUTPUT);
  
       }
       if(station2_status=="OFF")
       {
        pinMode(Y_STEP_PIN,   INPUT);
        pinMode(Y_DIR_PIN,    INPUT);
        pinMode(Y_ENABLE_PIN, INPUT);
       }
       //////////////
       String string_station3=data.substring(station3_index+9,station4_index);
       station3_status=string_station3;
       Serial.print("station3_status=");
       Serial.println(station3_status);
       if(station3_status=="ON")
       {
        pinMode(Z_STEP_PIN,   OUTPUT);
        pinMode(Z_DIR_PIN,    OUTPUT);
        pinMode(Z_ENABLE_PIN, OUTPUT);
       }
       if(station3_status=="OFF")
       {
        pinMode(Z_STEP_PIN,   INPUT);
        pinMode(Z_DIR_PIN,    INPUT);
        pinMode(Z_ENABLE_PIN, INPUT);
       }
       /////////////
       String string_station4=data.substring(station4_index+9,station5_index);
       station4_status=string_station4;
       Serial.print("station4_status=");
       Serial.println(station4_status);
       if(station4_status=="ON")
       {
        pinMode(A_STEP_PIN,   OUTPUT);
        pinMode(A_DIR_PIN,    OUTPUT);
        pinMode(A_ENABLE_PIN, OUTPUT);
       }
       if(station4_status=="OFF")
       {
        pinMode(A_STEP_PIN,   INPUT);
        pinMode(A_DIR_PIN,    INPUT);
        pinMode(A_ENABLE_PIN, INPUT);
       }
       /////////////
       String string_station5=data.substring(station5_index+9);
       station5_status=string_station5;
       Serial.print("station5_status=");
       Serial.println(station5_status);
       if(station5_status=="ON")
       {
        pinMode(B_STEP_PIN,   OUTPUT);
        pinMode(B_DIR_PIN,    OUTPUT);
        pinMode(B_ENABLE_PIN, OUTPUT);
       }
       if(station5_status=="OFF")
       {
        pinMode(B_STEP_PIN,   INPUT);
        pinMode(B_DIR_PIN,    INPUT);
        pinMode(B_ENABLE_PIN, INPUT);
       }
       /////////////


 /////------------------------------------------------------------------------------------------ 
         if(station1_status=="ON"|| station2_status=="ON" || station3_status=="ON" ||station4_status=="ON" || station5_status=="ON")
          {
             angle_diffz = (stop_angle-start_angle); // find angle diff
             start_angle1=(start_angle*1.1111); // start and stop angle for FOR loop (out of 20000 steps)
             stop_angle1=(stop_angle*1.1111);
             angle_diff1=angle_diffz * 1.1111;
             cycle_count1=cycle_count; // how many cycles to operate for.
             negativeAng=angle_diff1 * -1;
             if(motor_speed==1) // if the user chooses SLOW aka "1"
              {
            
                   
                  if(angle_diffz>=0 && angle_diffz<=45) // if angle is between 0 - 45
                    {
                      //WE THE FUCKING BEST
                      steppers[0].acceleration=18150;//DONE
                      steppers[1].acceleration=18150; //DONE
                      steppers[2].acceleration=18150; //DONE
                      steppers[3].acceleration=18150; //DONE
                      steppers[4].acceleration=18150; //DONE
                      
                    }
                  if(angle_diffz>45 && angle_diffz<=90) // if angle is between 45-90
                    {
                       //WTB
                       steppers[0].acceleration=12400; //DONE
                       steppers[1].acceleration=12400; //DONE
                       steppers[2].acceleration=12400; //DONE
                       steppers[3].acceleration=12400; //DONE
                       steppers[4].acceleration=12400; //DONE
                    }
                  if(angle_diffz>90 && angle_diffz<=135) // if angle is between 90-135
                    {
                       //WE THE BEST
                       steppers[0].acceleration=9900; //DONE
                       steppers[1].acceleration=9900; // DONE
                       steppers[2].acceleration=9900; // DONE
                       steppers[3].acceleration=9900; // DONE
                       steppers[4].acceleration=9900; // DONE
                    }
                  if(angle_diffz>135 && angle_diffz<=180) //if angle is between 135 and 180
                    {
                        //WE THE BEST
                        steppers[0].acceleration=8450; /// DONE 
                        steppers[1].acceleration=8450; /// DONE 
                        steppers[2].acceleration=8450; /// DONE 
                        steppers[3].acceleration=8450; /// DONE 
                        steppers[4].acceleration=8450; /// DONE 
                    }
                
                
               } // end of 1
               
           if(motor_speed==2) // if the user chooses MED aka "2"
              {
            
                   
                  if(angle_diffz>=0 && angle_diffz<=45) // if angle is between 0 - 45
                    {
                      //WE THE FUCKING BEST
                      steppers[0].acceleration=13600;//DONE
                      steppers[1].acceleration=13600; //DONE
                      steppers[2].acceleration=13600; //DONE
                      steppers[3].acceleration=13600; //DONE
                      steppers[4].acceleration=13600; //DONE
                      
                    }
                  if(angle_diffz>45 && angle_diffz<=90) // if angle is between 45-90
                    {
                       //WTB
                       steppers[0].acceleration=9250; //DONE
                       steppers[1].acceleration=9250; //DONE
                       steppers[2].acceleration=9250; //DONE
                       steppers[3].acceleration=9250; //DONE
                       steppers[4].acceleration=9250; //DONE
                    }
                  if(angle_diffz>90 && angle_diffz<=135) // if angle is between 90-135
                    {
                       //WE THE BEST
                       steppers[0].acceleration=7350; //DONE
                       steppers[1].acceleration=7350; // DONE
                       steppers[2].acceleration=7350; // DONE
                       steppers[3].acceleration=7350; // DONE
                       steppers[4].acceleration=7350; // DONE
                    }
                  if(angle_diffz>135 && angle_diffz<=180) //if angle is between 135 and 180
                    {
                        //WE THE BEST
                        steppers[0].acceleration=6250; /// DONE 
                        steppers[1].acceleration=6250; /// DONE 
                        steppers[2].acceleration=6250; /// DONE 
                        steppers[3].acceleration=6250; /// DONE
                        steppers[4].acceleration=6250; /// DONE 
                    }
                
                
               } // end of 2
               
             if(motor_speed==3) // if the user chooses FAST aka "3"
              {
            
                  if(angle_diffz>=0 && angle_diffz<=45) // if angle is between 0 - 45
                    {
                      //WE THE FUCKING BEST
                      steppers[0].acceleration=9000;//DONE
                      steppers[1].acceleration=9000; //DONE
                      steppers[2].acceleration=9000; //DONE
                      steppers[3].acceleration=9000; //DONE
                      steppers[4].acceleration=9000; //DONE
                      
                    }
                  if(angle_diffz>45 && angle_diffz<=90) // if angle is between 45-90
                    {
                       //WTB
                       steppers[0].acceleration=6100; //DONE
                       steppers[1].acceleration=6100; //DONE
                       steppers[2].acceleration=6100; //DONE
                       steppers[3].acceleration=6100; //DONE
                       steppers[4].acceleration=6100; //DONE
                    }
                  if(angle_diffz>90 && angle_diffz<=135) // if angle is between 90-135
                    {
                       //WE THE BEST
                       steppers[0].acceleration=4850; //DONE
                       steppers[1].acceleration=4850; // DONE
                       steppers[2].acceleration=4850; // DONE
                       steppers[3].acceleration=4850; // DONE
                       steppers[4].acceleration=4850; // DONE
                    }
                  if(angle_diffz>135 && angle_diffz<=180) //if angle is between 135 and 180
                    {
                        //WE THE BEST
                        steppers[0].acceleration=4050; /// DONE 
                        steppers[1].acceleration=4050; /// DONE 
                        steppers[2].acceleration=4050; /// DONE 
                        steppers[3].acceleration=4050; /// DONE 
                        steppers[4].acceleration=4050; /// DONE 
                    }
                
                
               } // end of 3
            
            
          }
          else
            {
            motor1_speed=0;
            start_angle1=0;
            stop_angle1=0;
            cycle_count1=0;

            motor2_speed=0;
            start_angle2=0;
            stop_angle2=0;
            cycle_count2=0;

            motor3_speed=0;
            start_angle3=0;
            stop_angle3=0;
            cycle_count3=0;

            motor4_speed=0;
            start_angle4=0;
            stop_angle4=0;
            cycle_count4=0;

            motor5_speed=0;
            start_angle5=0;
            stop_angle5=0;
            cycle_count5=0;
            }
      }
     }
   }
}

//////////////////////////////////////////////////////////////////
/*
void progress()
{
serial_data();
if(s1==true)
    {
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz1); 
      Serial1.print("\"");
            
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

   else if(s2==true)
    {
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz2); 
      Serial1.print("\"");      
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

    else if(s3==true)
    {
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz3); 
      Serial1.print("\"");      
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

    else if(s4==true)
    {
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz4); 
      Serial1.print("\"");      
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

    else if(s5==true)
    {
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz5); 
      Serial1.print("\"");      
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

}
*/
void progress()
{
serial_data();
if(s1==true && station1_status=="ON")
    {
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz1); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);      
      Serial1.print("t0.txt=");
      Serial1.print("\""); 
      Serial1.print(s[0].R); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

   else if(s2==true && station2_status=="ON")
    {      
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz2); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);      
      Serial1.print("t0.txt=");
      Serial1.print("\""); 
      Serial1.print(s[1].R); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

    else if(s3==true && station3_status=="ON")
    {      
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz3); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);      
      Serial1.print("t0.txt=");
      Serial1.print("\""); 
      Serial1.print(s[2].R); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

    else if(s4==true && station4_status=="ON")
    {      
      Serial1.print("t3.txt=");
      Serial1.write("\""); 
      Serial1.print(ctrz4); 
      Serial1.write("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);      
      Serial1.write("t0.txt=");
      Serial1.write("\""); 
      Serial1.print(s[3].R); 
      Serial1.write("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

    else if(s5==true && station5_status=="ON")
    {      
      Serial1.print("t3.txt=");
      Serial1.print("\""); 
      Serial1.print(ctrz4); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);      
      Serial1.print("t0.txt=");
      Serial1.print("\""); 
      Serial1.print(s[4].R); 
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }

}
//////////////////////////
//OHMMETER
//////////////////////////

bool ohmmeter ()
{
  //Serial.println("WTFB");
  //sensorValue0 = analogRead(sensorPin0);
  int i=0;
      switch(i){
        case 0: 
          sensorPin=A5;
          break;
        case 1:
          sensorPin=A7;
          break;
        case 2:
          sensorPin=A9;
          break;
        case 3:
          sensorPin=A11;
          break;
        case 4:
          sensorPin=A13;
          break;
          }
      if(s[i].flag==false)
      {
        float temp;
        temp=s[i].R;
        float p20= temp * 1.20;
        float m20= temp * .80;
        s[i].sensorValue=analogRead(sensorPin);
        s[i].Vout=(s[i].Vin * s[i].sensorValue)/1023;
        s[i].R = s[i].Rref * (1/((s[i].Vin / s[i].Vout)-1));
        if(s[i].R>p20 || s[i].R<m20 || s[i].sensorValue==1023)
          { //Serial.print("WOW1");
            if(s[i].z==0)
              { //Serial.print("WOW2");
                s[i].flag=true;
              }

          }
        if(s[i].z==1)
          {
            s[i].z=0;
            //Serial.print("DONE");
          }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("value:");
      Serial.println(s[i].R);
     
      //delay(1000);
      /*
      Serial.println("******");
      Serial.println(i);
      Serial.println("******");
      Serial.println(s[i].R);
      */
      ctrz1=counter1;
      return true;
      
      }
      
      else 
      {
        
        if(i==0)
        {
          //Serial.println("SHITBROKE0");
          pinMode(X_STEP_PIN,   INPUT);
          pinMode(X_DIR_PIN,    INPUT);
          pinMode(X_ENABLE_PIN, INPUT);
        }
        
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("CABLE BROKEN");
      //Serial.println();
      //delay(1000);
      //Serial.print("CABLE BROKEN");
      return false;
      }
   
    
}

bool ohmmeterA ()
{
  //Serial.println("WTFB");
  //sensorValue0 = analogRead(sensorPin0);
  int i=1;
      switch(i){
        case 0: 
          sensorPin=A5;
          break;
        case 1:
          sensorPin=A7;
          break;
        case 2:
          sensorPin=A9;
          break;
        case 3:
          sensorPin=A11;
          break;
        case 4:
          sensorPin=A13;
          break;
          }
      if(s[i].flag==false) //s[i].flag
     
      {
        float temp;
        temp=s[i].R;
        float p20= temp * 1.20;
        float m20= temp * .80;
        s[i].sensorValue=analogRead(sensorPin);
        s[i].Vout=(s[i].Vin * s[i].sensorValue)/1023;
        s[i].R = s[i].Rref * (1/((s[i].Vin / s[i].Vout)-1));
        if(s[i].R>p20 || s[i].R<m20 || s[i].sensorValue==1023)
          { //Serial.print("WOW1");
            if(s[i].z==0)
              { //Serial.print("WOW2");
                s[i].flag=true;
              }

          }
        if(s[i].z==1)
          {
            s[i].z=0;
            //Serial.print("DONE");
          }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("value:");
      //Serial.println(s[i].R);
     
      //delay(1000);
      /*
      Serial.println("******");
      Serial.println(i);
      Serial.println("******");*/
      Serial.println(s[i].R);
     
      ctrz2=counter1;
      return true;
      
      }
      
      else 
      {
        
        
        if(i==1)
        {
          //Serial.println("SHITBROKE1");
          pinMode(Y_STEP_PIN,   INPUT);
          pinMode(Y_DIR_PIN,    INPUT);
          pinMode(Y_ENABLE_PIN, INPUT);
        }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("CABLE BROKEN");
      //Serial.println();
      //delay(1000);
      //Serial.print("CABLE BROKEN");
      return false;
      }
   
    
}

bool ohmmeterB ()
{
  //Serial.println("WTFB");
  //sensorValue0 = analogRead(sensorPin0);
  int i=2;
      switch(i){
        case 0: 
          sensorPin=A5;
          break;
        case 1:
          sensorPin=A7;
          break;
        case 2:
          sensorPin=A9;
          break;
        case 3:
          sensorPin=A11;
          break;
        case 4:
          sensorPin=A13;
          break;
          }
      if(s[i].flag==false) //s[i].flag
     
      {
        float temp;
        temp=s[i].R;
        float p20= temp * 1.20;
        float m20= temp * .80;
        s[i].sensorValue=analogRead(sensorPin);
        s[i].Vout=(s[i].Vin * s[i].sensorValue)/1023;
        s[i].R = s[i].Rref * (1/((s[i].Vin / s[i].Vout)-1));
        if(s[i].R>p20 || s[i].R<m20 || s[i].sensorValue==1023)
          { //Serial.print("WOW1");
            if(s[i].z==0)
              { //Serial.print("WOW2");
                s[i].flag=true;
              }

          }
        if(s[i].z==1)
          {
            s[i].z=0;
            //Serial.print("DONE");
          }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("value:");
      Serial.println(s[i].R);
     
      //delay(1000);
      /*
      Serial.println("******");
      Serial.println(i);
      Serial.println("******");
      Serial.println(s[i].R);
      */
      ctrz3=counter1;
      return true;
      
      }
      
      else 
      {
        
        
        if(i==2)
        {
          //Serial.println("SHITBROKE2");
          pinMode(Z_STEP_PIN,   INPUT);
          pinMode(Z_DIR_PIN,    INPUT);
          pinMode(Z_ENABLE_PIN, INPUT);
        }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("CABLE BROKEN");
      //Serial.println();
      //delay(1000);
      //Serial.print("CABLE BROKEN");
      return false;
      }
   
    
}

bool ohmmeterC ()
{
  //Serial.println("WTFB");
  //sensorValue0 = analogRead(sensorPin0);
  int i=3;
      switch(i){
        case 0: 
          sensorPin=A5;
          break;
        case 1:
          sensorPin=A7;
          break;
        case 2:
          sensorPin=A9;
          break;
        case 3:
          sensorPin=A11;
          break;
        case 4:
          sensorPin=A13;
          break;
          }
      if(s[i].flag==false) //s[i].flag
     
      {
        float temp;
        temp=s[i].R;
        float p20= temp * 1.20;
        float m20= temp * .80;
        s[i].sensorValue=analogRead(sensorPin);
        s[i].Vout=(s[i].Vin * s[i].sensorValue)/1023;
        s[i].R = s[i].Rref * (1/((s[i].Vin / s[i].Vout)-1));
        if(s[i].R>p20 || s[i].R<m20 || s[i].sensorValue==1023)
          { //Serial.print("WOW1");
            if(s[i].z==0)
              { //Serial.print("WOW2");
                s[i].flag=true;
              }

          }
        if(s[i].z==1)
          {
            s[i].z=0;
            //Serial.print("DONE");
          }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("value:");
      Serial.println(s[i].R);
     
      //delay(1000);
      /*
      Serial.println("******");
      Serial.println(i);
      Serial.println("******");
      Serial.println(s[i].R);
      */
      ctrz4=counter1;
      return true;
      
      }
      
      else 
      {
        
        
        if(i==3)
        {
          //Serial.println("SHITBROKE3");
          pinMode(A_STEP_PIN,   INPUT);
          pinMode(A_DIR_PIN,    INPUT);
          pinMode(A_ENABLE_PIN, INPUT);
        }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("CABLE BROKEN");
      //Serial.println();
      //delay(1000);
      //Serial.print("CABLE BROKEN");
      return false;
      }
   
    
}
bool ohmmeterD ()
{
  //Serial.println("WTFB");
  //sensorValue0 = analogRead(sensorPin0);
  int i=4;
      switch(i){
        case 0: 
          sensorPin=A5;
          break;
        case 1:
          sensorPin=A7;
          break;
        case 2:
          sensorPin=A9;
          break;
        case 3:
          sensorPin=A11;
          break;
        case 4:
          sensorPin=A13;
          break;
          }
      if(s[i].flag==false) //s[i].flag
     
      {
        float temp;
        temp=s[i].R;
        float p20= temp * 1.20;
        float m20= temp * .80;
        s[i].sensorValue=analogRead(sensorPin);
        s[i].Vout=(s[i].Vin * s[i].sensorValue)/1023;
        s[i].R = s[i].Rref * (1/((s[i].Vin / s[i].Vout)-1));
        if(s[i].R>p20 || s[i].R<m20 || s[i].sensorValue==1023)
          { //Serial.print("WOW1");
            if(s[i].z==0)
              { //Serial.print("WOW2");
                s[i].flag=true;
              }

          }
        if(s[i].z==1)
          {
            s[i].z=0;
            //Serial.print("DONE");
          }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("value:");
      Serial.println(s[i].R);
     
      //delay(1000);
      /*
      Serial.println("******");
      Serial.println(i);
      Serial.println("******");
      Serial.println(s[i].R);
      */
      ctrz5=counter1;
      return true;
      
      }
      
      else 
      {
        
        
        if(i==4)
        {
          //Serial.println("SHITBROKE3");
          pinMode(B_STEP_PIN,   INPUT);
          pinMode(B_DIR_PIN,    INPUT);
          pinMode(B_ENABLE_PIN, INPUT);
        }
      //Serial.print("station __");
      //Serial.print(i);
      //Serial.print("CABLE BROKEN");
      //Serial.println();
      //delay(1000);
      //Serial.print("CABLE BROKEN");
      return false;
      }
   
    
}
