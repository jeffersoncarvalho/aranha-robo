/*************************************************** 
 * This is an example for our Adafruit 16-channel PWM & Servo driver
 * Servo test - this will drive 16 servos, one after the other
 * 
 * Pick one up today in the adafruit shop!
 * ------> http://www.adafruit.com/products/815
 * 
 * These displays use I2C to communicate, 2 pins are required to  
 * interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>

//string max size
#define INPUT_SIZE 400
//in pulses
#define SERVO_MAX 540
#define SERVO_CENTER 340
#define SERVO_MIN 140
//in degrees
#define SERVO_MAX_LIFT 130
#define SERVO_MIN_LIFT 50
#define SERVO_MAX_PIVOT 160
#define SERVO_MIN_PIVOT 20
//lift and pivot constants
#define SERVO_LIFT 1
#define SERVO_PIVOT 0
#define SERVO_RIGHT 2
#define SERVO_LEFT 4
#define SERVO_FRONT 6
#define SERVO_BACK 8
//servo pins on ada board
#define SERVO1 4
#define SERVO2 5
#define SERVO3 6
#define SERVO4 7
#define SERVO5 8
#define SERVO6 9
#define SERVO7 10
#define SERVO8 11
//constants for string input
#define SINGLE_SERVO 10
#define LEAN_RIGTH 30
#define LEAN_LEFT 40
#define LEAN_FRONT 80
#define LEAN_BACK 90
#define MOVE_FOUR_SERVOS_LIFT 2468
#define MOVE_FOUR_SERVOS_PIVOT 1357 
#define MOVE_SCISSOR_RIGTH 50
#define MOVE_SCISSOR_LEFT 60
#define DELAY 1000
#define SHUTDOWN 666
//servo global variables
int speed_servo = 0;
int centers[12] = {-1,-1,-1,90,90,90,90,90,90,90,90,90};
int calibrations[12] = {-1,-1,-1,0, 0,0,0,-10,-10,+10,5,0};
int pos[12] = {-1,-1,-1,90,90,90,90,90,90,90,90,90};
int shutdown_counter = 10;
//iniatializing hardware
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SoftwareSerial mySerial(0, 1); // RX, TX



void setup(){
  mySerial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
}

int turn = 0;
void loop(){
  if(turn==0){
    move_servo(3,90);
    center_servos_();
    turn = 1;
    delay(2000);
    //shutdown();
    //delay(2000);
  }
  //battery_test();
  //timedAction.check(); 
  //inspect();
  high_level_menu();
  //low_level_menu();
  //battery_test();
}

void lean_(int type){
  center_servos_();
  switch(type){
    case SERVO_FRONT:
      move_pair_lifts(SERVO_MAX_LIFT,SERVO_BACK);
      move_pair_lifts(SERVO_MIN_LIFT,SERVO_FRONT);
    break;
    case SERVO_BACK:
      move_pair_lifts(SERVO_MAX_LIFT,SERVO_FRONT);
      move_pair_lifts(SERVO_MIN_LIFT,SERVO_BACK);
    break;
    case SERVO_LEFT:
      move_pair_lifts(SERVO_MAX_LIFT,SERVO_RIGHT);
      move_pair_lifts(SERVO_MIN_LIFT,SERVO_LEFT);
    break;
    case SERVO_RIGHT:
      move_pair_lifts(SERVO_MAX_LIFT,SERVO_LEFT);
      move_pair_lifts(SERVO_MIN_LIFT,SERVO_RIGHT);
    break;
  }
  
}

/*center servos in position 90 degrees*/
void center_servos_(){
   char in[] = "1:90&2:90&3:90&4:90&5:90&6:90&7:90&8:90";
   split_serial(in);
   
}
void center_servos_fast_(){
  int pulse1 = map(90+calibrations[SERVO1], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse2 = map(90+calibrations[SERVO2], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse3 = map(90+calibrations[SERVO3], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse4 = map(90+calibrations[SERVO4], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse5 = map(90+calibrations[SERVO5], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse6 = map(90+calibrations[SERVO6], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse7 = map(90+calibrations[SERVO7], 0, 180, SERVO_MIN, SERVO_MAX);
  int pulse8 = map(90+calibrations[SERVO8], 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(SERVO1,0,pulse1);        
  pwm.setPWM(SERVO2,0,pulse2);
  pwm.setPWM(SERVO3,0,pulse3);
  pwm.setPWM(SERVO4,0,pulse4);
  pwm.setPWM(SERVO5,0,pulse5);        
  pwm.setPWM(SERVO6,0,pulse6);
  pwm.setPWM(SERVO7,0,pulse7);
  pwm.setPWM(SERVO8,0,pulse8);
  
  pos[SERVO1] = 90+calibrations[SERVO1];
  pos[SERVO2] = 90+calibrations[SERVO2];
  pos[SERVO3] = 90+calibrations[SERVO3];
  pos[SERVO4] = 90+calibrations[SERVO4];
  pos[SERVO5] = 90+calibrations[SERVO5];
  pos[SERVO6] = 90+calibrations[SERVO6];
  pos[SERVO7] = 90+calibrations[SERVO7];
  pos[SERVO8] = 90+calibrations[SERVO8];


}

/*move forward*/
void foward_(){
  char in[] = "4:50&60:160&4:90&2:50&60:90&2:90&6:50&50:160&6:90&8:50&50:90&8:90"; 
  split_serial(in);
}
/*move back*/
void back_(){
  char in[] = "2:50&60:160&2:90&4:50&60:90&4:90&8:50&50:160&8:90&6:50&50:90&6:90";
  split_serial(in); 
}
/*turn right*/
void turn_right_(){
  //new version
   char in[] = "1357:130&4:30&8:50&7:50&4:90&8:90&6:50&5:50&6:90&4:50&3:50&4:90&6:30&4:30&2:50&1:50&6:90&4:90&2:90&1357:90";
   //char in[] = "1357:130&8:50&7:50&8:90&6:50&5:50&6:90&4:50&3:50&4:90&2:50&1:50&2:90&1357:90";
   split_serial(in);
}

/*turn left*/
void turn_left_(){
  //new version
   char in[] = "1357:50&6:30&8:30&2:50&1:130&6:90&8:90&2:90&4:50&3:130&4:90&6:50&5:130&6:90&4:30&8:50&7:130&4:90&8:90&1357:90";
   //char in[] = "1357:50&2:50&1:130&2:90&4:50&3:130&4:90&6:50&5:130&6:90&8:50&7:130&8:90&1357:90";
   split_serial(in);
   
}

void wave_(){
  
  lean_(SERVO_BACK);
  delay(500);
  move_servo(SERVO2,SERVO_MIN_LIFT);
  delay(500);
  int i;
  for(i=0;i<6;i++){
    if(i%2==0) move_servo(SERVO1,140);
    else move_servo(SERVO1,40);
  }
  delay(100);
  move_servo(SERVO1,90);
  delay(100);
  move_servo(SERVO2,90);
  delay(100);
  move_servo(SERVO8,90);
  delay(100);
  move_pair_lifts(90,SERVO_BACK);
}

void inspect(){
  foward_();
  delay(1000);
  foward_();
  delay(1000);
  back_();
  delay(1000);
  back_();
  delay(1000);
  turn_right_();
  delay(1000);
  turn_right_();
  delay(1000);
  turn_left_();
  delay(1000);
  turn_left_();
  delay(1000);
  wave_();
  delay(1000);
  
}

void little_square_of_anitta(){
  
  move_servo(SERVO5,30);
  move_servo(SERVO3,150);
  delay(500);
  move_servo(SERVO2,SERVO_MAX_LIFT);
  move_servo(SERVO8,SERVO_MAX_LIFT);
  delay(500);
  move_servo(SERVO4,SERVO_MAX_LIFT);
  move_servo(SERVO6,SERVO_MAX_LIFT);
  int i = 0;
  for(i=0;i<5;i++){
    move_servo(SERVO4,SERVO_MAX_LIFT);
    delay(500);
    move_servo(SERVO6,SERVO_MAX_LIFT);
    delay(500);
    move_servo(SERVO4,SERVO_MIN_LIFT);
    delay(500);
    move_servo(SERVO6,SERVO_MIN_LIFT);
    delay(500);
  }
  
  center_servos_fast_();
   
  
}

void battery_test(){
  while(1){
   delay(1000);
   wave_();
   delay(1000);
   sit_();
   delay(1000);
   center_servos_();
   delay(1000);
   stand_();
   delay(1000);
   sit_();
   delay(1000);
   center_servos_();
   delay(1000);
   lean_(SERVO_FRONT);
   delay(1000);
   lean_(SERVO_BACK);
   delay(1000);
   lean_(SERVO_LEFT);
   delay(1000);
   lean_(SERVO_RIGHT);
   delay(1000);
   center_servos_();
   delay(1000);
   turn_right_();
   delay(1000);
   turn_left_();
  }
}

/*stand at once*/
void stand_(){
   move_4_servos(SERVO_MAX_LIFT,SERVO_LIFT);
}
/*sit at once*/
void sit_(){
   move_4_servos(SERVO_MIN_LIFT,SERVO_LIFT);
}
/*counter for shutdown*/
void counter(){
  if(shutdown_counter>0)
    shutdown_counter = shutdown_counter - 1;
  if(shutdown_counter==0)
    shutdown();
}
/*shutdown servos*/
void shutdown(){
    pwm.setPWM(SERVO1,0,0);        
    pwm.setPWM(SERVO2,0,0);
    pwm.setPWM(SERVO3,0,0);
    pwm.setPWM(SERVO4,0,0);
    pwm.setPWM(SERVO5,0,0);        
    pwm.setPWM(SERVO6,0,0);
    pwm.setPWM(SERVO7,0,0);
    pwm.setPWM(SERVO8,0,0);
}

/****MOVEMENT ROUTINES****/

/*move servo to fin position in degrees-servo number must be equals to servo pin*/
void move_servo(int servo, int fin){
  //delay(0);
  
  //calibration
  int ini = pos[servo];
  fin = fin + calibrations[servo];
  
  /*if(fin>170+calibrations[servo-SHIFT_FACTOR])
    fin = 170+calibrations[servo-SHIFT_FACTOR];
  if(fin<10+calibrations[servo-SHIFT_FACTOR])
    fin = 10+calibrations[servo-SHIFT_FACTOR];*/
  
  //mapping to pulse  
  int i = 0;
  int pf = map(fin, 0, 180, SERVO_MIN, SERVO_MAX);
  int pi = map(ini, 0, 180, SERVO_MIN, SERVO_MAX);

  if(pf>=pi)
    for(i=pi;i<=pf;i++){
      pwm.setPWM(servo,0,i);
      delay(speed_servo);  
    }
  else
    for(i=pi;i>=pf;i--){
      pwm.setPWM(servo,0,i);
      delay(speed_servo);    
    }
  
  pos[servo] = fin;

}

/*move two pivots in a scissor fashion, according to servoa final position*/
void move_scissor(int final,int type){
  int servoa = SERVO1;
  int servob = SERVO3;
  if(type == SERVO_RIGHT){
    servoa = SERVO5;
    servob = SERVO7;
  }else if(type == SERVO_FRONT){
    servoa = SERVO1;
    servob = SERVO7;
  }else if(type == SERVO_BACK){
    servoa = SERVO3;
    servob = SERVO5;
  }
  
  int fin1 = final + calibrations[servoa];
  int ini1 = pos[servoa];
  int ini2 = pos[servob];
  
  int fina,finb;
  fina = ini1+(fin1-ini1);
  finb = ini2-(fin1-ini1);
  
  
  
  
  int pos1map = map(ini1, 0, 180, SERVO_MIN, SERVO_MAX);
  int pos2map = map(ini2, 0, 180, SERVO_MIN, SERVO_MAX);
  
 
  int fin1map = map(fin1, 0, 180, SERVO_MIN, SERVO_MAX);
  
  
  if(fin1map>pos1map){
    int dif = fin1map-pos1map;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      pos1map = pos1map + 1;
      pos2map = pos2map - 1;
      pwm.setPWM(servoa,0,pos1map);
      pwm.setPWM(servob,0,pos2map);
      //delay(1);    
    }
  }else{
    int dif = pos1map-fin1map;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      pos1map = pos1map - 1;
      pos2map = pos2map + 1;
      pwm.setPWM(servoa,0,pos1map);
      pwm.setPWM(servob,0,pos2map);
      //delay(1);    
    }
  }
  
  pos[servoa] = fina; 
  pos[servob] = finb;
  
}

/*moves four servos at once to final position, in degrees. Type can be lift or pivot.*/
void move_4_servos(int final,int type){
  
  int servoa = SERVO1;
  int servob = SERVO3;
  int servoc = SERVO5;
  int servod = SERVO7;
  
  if(type == SERVO_LIFT){
    servoa = SERVO2;
    servob = SERVO4;
    servoc = SERVO6;
    servod = SERVO8;
  }
  
  int fin1 = final + calibrations[servoa];
  
  int ini1 = pos[servoa];
  int ini2 = pos[servob];
  int ini3 = pos[servoc];
  int ini4 = pos[servod];

  //mapping to pulse
  int pos1map = map(ini1, 0, 180, SERVO_MIN, SERVO_MAX);
  int pos2map = map(ini2, 0, 180, SERVO_MIN, SERVO_MAX);
  int pos3map = map(ini3, 0, 180, SERVO_MIN, SERVO_MAX);
  int pos4map = map(ini4, 0, 180, SERVO_MIN, SERVO_MAX);
  
  int fin1map = map(fin1, 0, 180, SERVO_MIN, SERVO_MAX);
  
  if(fin1map>pos1map){
    int dif = fin1map-pos1map;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      pos1map = pos1map + 1;
      pos2map = pos2map + 1;
      pos3map = pos3map + 1;
      pos4map = pos4map + 1;
      pwm.setPWM(servoa,0,pos1map);
      pwm.setPWM(servob,0,pos2map);
      pwm.setPWM(servoc,0,pos3map);
      pwm.setPWM(servod,0,pos4map);
      //delay(1);    
    }
  }else{
    int dif = pos1map-fin1map;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      pos1map = pos1map - 1;
      pos2map = pos2map - 1;
      pos3map = pos3map - 1;
      pos4map = pos4map - 1;
      pwm.setPWM(servoa,0,pos1map);
      pwm.setPWM(servob,0,pos2map);
      pwm.setPWM(servoc,0,pos3map);
      pwm.setPWM(servod,0,pos4map);
      //delay(1);    
    }
  }
  
  pos[servoa] = final+calibrations[servoa]; 
  pos[servob] = final+calibrations[servob];
  pos[servoc] = final+calibrations[servoc]; 
  pos[servod] = final+calibrations[servod];
  
}

/*move two lift legs at once: front, back, left or right*/
void move_pair_lifts(int final,int type){
  
  int servoa = SERVO2;
  int servob = SERVO4;
  if(type == SERVO_RIGHT){
    servoa = SERVO6;
    servob = SERVO8;
  }else if(type == SERVO_FRONT){
    servoa = SERVO2;
    servob = SERVO8;
  }else if(type == SERVO_BACK){
    servoa = SERVO4;
    servob = SERVO6;
  }
  
  int fin1 = final + calibrations[servoa];
  
  int ini1 = pos[servoa];
  int ini2 = pos[servob];
  

  //mapping to pulse
  int pos1map = map(ini1, 0, 180, SERVO_MIN, SERVO_MAX);
  int pos2map = map(ini2, 0, 180, SERVO_MIN, SERVO_MAX);
  
  
  int fin1map = map(fin1, 0, 180, SERVO_MIN, SERVO_MAX);
  
  if(fin1map>pos1map){
    int dif = fin1map-pos1map;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      pos1map = pos1map + 1;
      pos2map = pos2map + 1;
      pwm.setPWM(servoa,0,pos1map);
      pwm.setPWM(servob,0,pos2map);
      //delay(1);    
    }
  }else{
    int dif = pos1map-fin1map;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      pos1map = pos1map - 1;
      pos2map = pos2map - 1;
      pwm.setPWM(servoa,0,pos1map);
      pwm.setPWM(servob,0,pos2map);
      //delay(1);    
    }
  }
  
  pos[servoa] = final+calibrations[servoa]; 
  pos[servob] = final+calibrations[servob];
  
}


/**READ STRING**/
void split_serial(char input[]){



  // Read each command pair 
  char* command = strtok(input, "&");
  while (command != 0)
  {
    // Split the command in two values
    char* separator = strchr(command, ':');
    if (separator != 0)
    {
      // Actually split the string in 2: replace ':' with 0
      *separator = 0;
      int const servoId = atoi(command);
      
   
      //*****one single servo
      if (servoId<SINGLE_SERVO){
        ++separator;
        int position = atoi(separator);
        // Do something with servoId and position
        move_servo(servoId+3,position);
      }
      //***** 4 servos at once - PIVOT
      else if(servoId == MOVE_FOUR_SERVOS_PIVOT){
        ++separator;
        int position = atoi(separator);
        // Do something with servoId and position
        move_4_servos(position,SERVO_PIVOT);
      }
      //***** 4 servos at once - LIFT
      else if (servoId==MOVE_FOUR_SERVOS_LIFT){
        ++separator;
        int position = atoi(separator);
        // Do something with servoId and position
        move_4_servos(position,SERVO_LIFT);
      }
      //*****scissor right side
      else if (servoId==MOVE_SCISSOR_RIGTH){
        ++separator;
        int position = atoi(separator);
        move_scissor(position,SERVO_RIGHT);
      }
      //*****scissor left side
      else if (servoId==MOVE_SCISSOR_LEFT){
        ++separator;
        int position = atoi(separator);
        move_scissor(position,SERVO_LEFT);
      }
      //*****lean lift legs right sde
      else if (servoId==LEAN_RIGTH){
        ++separator;
        int position = atoi(separator);
        lean_(SERVO_RIGHT);
      }
      //*****lean lift legs right sde
      else if (servoId==LEAN_LEFT){
        ++separator;
        int position = atoi(separator);
        lean_(SERVO_LEFT);
      }
      //*****lean lift legs right sde
      else if (servoId==LEAN_FRONT){
        ++separator;
        int position = atoi(separator);
        lean_(SERVO_FRONT);
      }
      //*****lean lift legs right sde
      else if (servoId==LEAN_BACK){
        ++separator;
        int position = atoi(separator);
        lean_(SERVO_BACK);
      }
      //*****delay ms
      else if (servoId==DELAY){
        ++separator;
        int position = atoi(separator);
        delay(position);
      
      }
      //shutdown servos motors
      else if (servoId==SHUTDOWN){
        shutdown();
      }
      
    }
    // Find the next command in input string
    command = strtok(0, "&");
  }
}

/**iteraction types*/

void high_level_menu(){
  
  if(mySerial.available()){
    
    //char input[INPUT_SIZE + 1];
    //byte size = mySerial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    //input[size] = 0;
    
    byte c = mySerial.read();
    //byte c = input[0];
    shutdown_counter = 10;
    switch(c){
      case 'w':
        foward_();
      break;
      case 's':
        back_();
      break;
      case 'a':
        turn_left_();
      break;
      case 'd':
        turn_right_();
      break;
      case 'q':
        sit_();
      break;
      case 'e':
        stand_();
      break;
      case 'r':
        center_servos_();
      break;
      case 't':
        center_servos_fast_();
      break;
      case 'b':
        wave_();
      break;
      case 'i':
        lean_(SERVO_FRONT);
      break;
      case 'o':
        lean_(SERVO_BACK);
      break;
      case 'j':
        lean_(SERVO_LEFT);
      break;
      case 'k':
        lean_(SERVO_RIGHT);
      break;
      case 'x':
        shutdown();
      break;
      case 'z':
        little_square_of_anitta();
      break;
      case 'g':
        battery_test();
      break;
    
    }
    
  }
}
  
  
void low_level_menu(){
  
  char input[INPUT_SIZE + 1];
  byte size = mySerial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[size] = 0;
  split_serial(input);
}



