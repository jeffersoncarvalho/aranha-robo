#include <Servo.h>
#include <SoftwareSerial.h>

//string max size
#define INPUT_SIZE 400
//in pulses
#define SERVO_MAX 540
#define SERVO_CENTER 340
#define SERVO_MIN 140
//in degrees
#define SERVO_MAX_LIFT 150
#define SERVO_MIN_LIFT 10
#define SERVO_MAX_PIVOT 170
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
#define LEAN_RIGHT 30
#define LEAN_LEFT 40
#define LEAN_FRONT 80
#define LEAN_BACK 90
#define MOVE_FOUR_SERVOS_PIVOT 1357 
#define MOVE_FOUR_SERVOS_LIFT 2468
#define MOVE_SCISSOR_RIGTH 50
#define MOVE_SCISSOR_LEFT 60
#define DELAY 1000
#define SHUTDOWN 666


Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;
Servo myservo7;
Servo myservo8;
Servo dummy;

float speed_servo = 1;
Servo servos[12] = {dummy,dummy,dummy,dummy,myservo1,myservo2,myservo3,myservo4,myservo5,myservo6,myservo7,myservo8};
int calibrations[12] = {-1,-1,-1,-1,0,0,0,-10,-10,-5,2,0};
int pos[12] = {-1,-1,-1,-1,90,90,90,90,90,90,90,90};
int shutdown_counter = 10;



SoftwareSerial mySerial(0, 1); // RX, TX
//TimedAction timedAction = TimedAction(1000,counter);


void setup(){
  mySerial.begin(9600);
  attach();
  
}

int turn = 0;
void loop(){
  if(turn==0){
    center_servos_();
    turn = 1;
    delay(200);
    //shutdown();
    delay(1000);
  }
  //timedAction.check(); 
  //inspect();
  high_level_menu();
  //low_level_menu();
  //battery_test();
  
  
  
  
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

/*center servos in position 90 degrees*/
void center_servos_(){
   char in[] = "1:90&2:90&3:90&4:90&5:90&6:90&7:90&8:90";
   split_serial(in);
   
}
/*move forward*/
void foward_(){
  char in[] = "4:10&60:170&4:90&2:10&60:90&2:90&6:10&50:140&6:90&8:10&50:90&8:90"; 
  //char in[] = "4:30&2:120&60:170&4:90&2:30&60:90&2:90&1000:50&6:30&8:120&50:170&6:90&8:30&50:90&8:90"; 
  split_serial(in);
}
/*move back*/
void back_(){
  char in[] = "2:10&60:170&2:90&4:10&60:90&4:90&8:10&50:170&8:90&6:10&50:90&6:90";
  split_serial(in); 
}
/*turn right*/
void turn_right_(){
   //char in[] = "1357:130&4:30&8:50&7:50&4:90&8:90&6:50&5:50&6:90&4:50&3:50&4:90&6:30&2:50&1:50&6:90&2:90&1357:90";
   char in[] = "1357:130&8:50&7:50&8:90&6:50&5:50&6:90&4:50&3:50&4:90&2:50&1:50&2:90&1357:90";
   split_serial(in);
}

/*turn left*/
void turn_left_(){
   //char in[] = "1357:50&6:30&2:50&1:130&6:90&2:90&4:50&3:130&4:90&6:50&5:130&6:90&4:30&8:50&7:130&4:90&8:90&1357:90";
   char in[] = "1357:50&2:50&1:130&2:90&4:50&3:130&4:90&6:50&5:130&6:90&8:50&7:130&8:90&1357:90";
   split_serial(in);

}

/*stand*/
void stand_(){
  char in[] = "2468:130";
  split_serial(in);
  //move_4_servos(130,SERVO_LIFT);
}

/*sit*/
void sit_(){
  char in[] = "2468:50";
  split_serial(in);
   //move_4_servos(50,SERVO_LIFT);
}

/*center servos fast*/
void center_servos_fast_(){
  int i=SERVO1;
  for (i=SERVO1;i<=SERVO8;i++){
    int pos90 = 90 + calibrations[i]; 
    servos[i].write(pos90);
    pos[i] = pos90;
  }
}

/*patrolling*/
void patrolling(int type){

   if(type == 0)
    stand_();
   else
    sit_();
   
   delay(500);
   move_4_servos(30,SERVO_PIVOT,25);
   delay(500);
   move_4_servos(90,SERVO_PIVOT,25);
   delay(500);
   move_4_servos(150,SERVO_PIVOT,25);
   delay(500);
   move_4_servos(90,SERVO_PIVOT,25);
   delay(500);
   center_servos_();

}

/*wave*/
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

void sleep(){
  move_servo(SERVO1,40);
  delay(250);
  move_servo(SERVO2,40);
  delay(250);
  move_servo(SERVO3,40);
  delay(250);
  move_servo(SERVO4,40);
  delay(250);
  move_servo(SERVO5,40);
  delay(250);
  move_servo(SERVO6,40);
  delay(250);
  move_servo(SERVO7,40);
  delay(250);
  move_servo(SERVO8,40);
}

void counter(){
  if(shutdown_counter>0)
    shutdown_counter = shutdown_counter - 1;
  if(shutdown_counter==0)
    shutdown();
}

void shutdown(){
  myservo1.detach();
  myservo2.detach();
  myservo3.detach();
  myservo4.detach();
  myservo5.detach();
  myservo6.detach();
  myservo7.detach();
  myservo8.detach();
}

void attach(){
  myservo1.attach(SERVO1);
  myservo2.attach(SERVO2);
  myservo3.attach(SERVO3);
  myservo4.attach(SERVO4);
  myservo5.attach(SERVO5);
  myservo6.attach(SERVO6);
  myservo7.attach(SERVO7);
  myservo8.attach(SERVO8);
}

/*MOVEMENT ROUTINES*/
void lean_(int type){
  center_servos_fast_();
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
  
  if(final>ini1){
    int dif = final-ini1;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      ini1 = ini1 + 1;
      ini2 = ini2 + 1;
      servos[servoa].write(ini1);
      servos[servob].write(ini2);
      delay(speed_servo);    
    }
  }else{
    int dif = ini1-final;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      ini1 = ini1 - 1;
      ini2 = ini2 - 1;
      servos[servoa].write(ini1);
      servos[servob].write(ini2);
      delay(speed_servo);    
    }
  }
  
  pos[servoa] = ini1;
  pos[servob] = ini2;
  
}

//moves a single servo to fin position in degrees.
void move_servo(int servo, int final){
  delay(50);
  
  if((servo == SERVO1) || (servo == SERVO3) || (servo == SERVO5) || (servo == SERVO7)){
    if(final<SERVO_MIN_PIVOT) final=SERVO_MIN_PIVOT;
    if(final>SERVO_MAX_PIVOT) final=SERVO_MAX_PIVOT;
  }else{
    if(final<SERVO_MIN_LIFT) final=SERVO_MIN_LIFT;
    if(final>SERVO_MAX_LIFT) final=SERVO_MAX_LIFT;
  }
  
  
  int ini = pos[servo];
  final = final + calibrations[servo];
  
  /*if(fin>170+calibrations[servo-SHIFT_FACTOR])
    fin = 170+calibrations[servo-SHIFT_FACTOR];
  if(fin<10+calibrations[servo-SHIFT_FACTOR])
    fin = 10+calibrations[servo-SHIFT_FACTOR];*/
  
  int i = 0;
  if(final>=ini)
    for(i=ini;i<=final;i++){
      servos[servo].write(i);
      delay(speed_servo);  
    }
  else
    for(i=ini;i>=final;i--){
      servos[servo].write(i);
      delay(speed_servo);    
    }
  
  pos[servo] = final;

}


// move side servos to final position
// based on front servos 1 (left side) or 7 (right side)
void move_scissor(int final,int type){

  if(final<SERVO_MIN_PIVOT) final=SERVO_MIN_PIVOT;
  if(final>SERVO_MAX_PIVOT) final=SERVO_MAX_PIVOT;
  
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
  int fin2 = final + calibrations[servob];
  
  int ini1 = pos[servoa];
  int ini2 = pos[servob];
  
  
  if(fin1>ini1){
    int dif = fin1-ini1;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      ini1 = ini1 + 1;
      ini2 = ini2 - 1;
      servos[servoa].write(ini1);
      servos[servob].write(ini2);
      delay(speed_servo);    
    }
  }else{
    int dif = ini1-fin1;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      ini1 = ini1 - 1;
      ini2 = ini2 + 1;
      servos[servoa].write(ini1);
      servos[servob].write(ini2);
      delay(speed_servo);    
    }
  }
  
  pos[servoa] = ini1; 
  pos[servob] = ini2;
  
}

//moves four servos at once to final position, in degrees.
void move_4_servos(int final,int type, float velocity){
  
  int servoa = SERVO1;
  int servob = SERVO3;
  int servoc = SERVO5;
  int servod = SERVO7;
  
  if(final<SERVO_MIN_PIVOT) final=SERVO_MIN_PIVOT;
  if(final>SERVO_MAX_PIVOT) final=SERVO_MAX_PIVOT;
  
  if(type == SERVO_LIFT){
    servoa = SERVO2;
    servob = SERVO4;
    servoc = SERVO6;
    servod = SERVO8;
    
    if(final<SERVO_MIN_LIFT) final=SERVO_MIN_LIFT;
    if(final>SERVO_MAX_LIFT) final=SERVO_MAX_LIFT;
    
  }
  
  
  int fin1 = final + calibrations[servoa];
  
  
  
  int ini1 = pos[servoa];
  int ini2 = pos[servob];
  int ini3 = pos[servoc];
  int ini4 = pos[servod];

  
  if(fin1>ini1){
    int dif = fin1-ini1;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      ini1 = ini1 + 1;
      ini2 = ini2 + 1;
      ini3 = ini3 + 1;
      ini4 = ini4 + 1;
      servos[servoa].write(ini1);
      servos[servob].write(ini2);
      servos[servoc].write(ini3);
      servos[servod].write(ini4);
      delay(velocity);    
    
    }
  }else{
    int dif = ini1-fin1;
    int i = 1;
    for(i=1;i<=dif;i+=1){
      ini1 = ini1 - 1;
      ini2 = ini2 - 1;
      ini3 = ini3 - 1;
      ini4 = ini4 - 1;
      servos[servoa].write(ini1);
      servos[servob].write(ini2);
      servos[servoc].write(ini3);
      servos[servod].write(ini4);
      delay(velocity);    
    
    }
  }
  
  pos[servoa] = ini1; 
  pos[servob] = ini2;
  pos[servoc] = ini3; 
  pos[servod] = ini4;
  
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
        move_4_servos(position,SERVO_PIVOT,speed_servo);
      }
      //***** 4 servos at once - LIFT
      else if (servoId==MOVE_FOUR_SERVOS_LIFT){
        ++separator;
        int position = atoi(separator);
        // Do something with servoId and position
        move_4_servos(position,SERVO_LIFT,speed_servo);
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
      else if (servoId==LEAN_RIGHT){
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
        //turn_right_s();
      break;
      case 'q':
        //mySerial.print("\nq");
        sit_();
      break;
      case 'e':
        //mySerial.print("\ne");
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
        sleep();
      break;
      case 'c':
        attach();
      break;
      case 'p':
        patrolling(0);
      break;
      case 'l':
        patrolling(1);
      break;
      case 'f':
        speed_servo = (speed_servo ==10)?10:speed_servo+0.2;
      break;
      case 'g':
        speed_servo = (speed_servo ==0)?0:speed_servo-0.2;
      break;
    }
    
    delay(250);
  }
}
  
  
void low_level_menu(){
  
  char input[INPUT_SIZE + 1];
  byte size = mySerial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[size] = 0;
  split_serial(input);
}


//SCRATCH...
/*void turn_right_s(){

  int wait = 1;
  int factor = 100;
  int factor_lift = 100;
  int x = 1;
  int lifts[4] = {SERVO6,SERVO2,SERVO4,SERVO8};
  int pivots[4] = {SERVO1,SERVO5,SERVO7,SERVO3};
  
  for(x=0;x<4;x++){
    move_servo(lifts[x],70);
    delay(wait);
    move_servo(pivots[x],70);
    delay(wait);
    move_servo(lifts[x],90);
    delay(wait);
    
    if((x+1)%2==0){
      if(x==3){
        move_servo(SERVO2,70);
        move_servo(SERVO6,70);
        delay(50);
      }
      
      move_servo(pivots[x-1],110);
      move_servo(pivots[x],110);
      delay(wait);
      
      if(x==3){
        move_servo(SERVO2,90);
        move_servo(SERVO6,90);
      }
    } 
  }
}*/




