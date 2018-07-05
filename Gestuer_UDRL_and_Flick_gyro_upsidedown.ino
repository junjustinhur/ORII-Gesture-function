#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "I2Cdev.h"
 
LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B

int tab = 0; // initialize tab 
int period = 500;

void setup() {
  Serial.begin(74880);
  delay(1000); 
  Serial.println("Processor came out of reset.\n");
 
  //Call .begin() to configure the IMU
  myIMU.begin();
}
 
   
void loop()
{
  // Right Left Up Down Shake------------------------------------------------------------------------------------------------
 
  if (myIMU.readFloatGyroX() > 700){ //Once myIMU.readFloatGyroX() exceeds 700 rad/s excecute for loop for 500ms
    int i =0;
    float a[2];
    int num;
    for (i; i < 500; i++){
      a[1] = myIMU.readFloatAccelY();
      if((a[1] > 2.5 && a[0] < 2.5) || (a[1] < -2.5 && a[0] > -2.5)){
        num = num + 1; // count how often myIMU.readFloatAccelY() exceeds either 2.5 or -2.5
        }
      a[0] = a[1];
      }
    if(num >= 4){ //if myIMU.readFloatAccelY() exceeds 2.5 or -2.5 more than 4 times, print "Shake"  and delay for 300ms
      Serial.print("~Shake~");
      Serial.print('\n');
      delay(300);
      }   
    else if (num <2){ //if myIMU.readFloatAccelY() exceeds 2.5 or -2.5 less than twice, print "Left"  and delay for 300ms
      Serial.println("Left");
      delay(300);
      }
    }
  
  if(myIMU.readFloatGyroX() < -700){ //Once myIMU.readFloatGyroX() exceeds -700 rad/s excecute for loop for 500ms
    int i =0;
    float a[2];
    int num;
    for (i; i < 500; i++){
      a[1] = myIMU.readFloatAccelY();
      if((a[1] > 2.5 && a[0] < 2.5) || (a[1] < -2.5 && a[0] > -2.5)){
        num = num + 1;
        }
      a[0] = a[1];
      }
    if(num >= 4){ //if myIMU.readFloatAccelY() exceeds 2.5 or -2.5 more than 4 times, print "Shake" and delay for 300ms
      Serial.print("~Shake~");
      Serial.print('\n');
      delay(300);
      }   
    else if(num < 2){ //if myIMU.readFloatAccelY() exceeds 2.5 or -2.5 less than twice, print "Right"  and delay for 300ms
      Serial.println("Right");
      delay(300);
      }
  } 

  if(myIMU.readFloatGyroY() > 700){ //If myIMU.readFloatGyroY() exceeds 700rad/s, print "Down" and delay for 300ms
    Serial.println("Down");
    delay(300);
    } 


  if(myIMU.readFloatGyroY() < -700){ //If myIMU.readFloatGyroY() exceeds -700 print "Up" and delay for 300ms
    Serial.println("Up");
    delay(300);
    } 
    
  // Flick Tap ------------------------------------------------------------------------------------------------
  // As the finger flicks, Z accelaration exceeds either -6 or 6. And after few miliseconds, X acceleration exceeds either -6 or 6.
  // When tapping on a hard surface with palm, Z accelration exceeds POSITIVE 6, and neither of Z acceleration and X acceleration exceeds negative 6
      
    if(myIMU.readFloatAccelZ() < -6 || myIMU.readFloatAccelZ() > 6){ // Once Z accelration exceeds either 6 or -6 execute a loop for 400ms.    
      int i =0;
      float z[2];
      float x[2];
      int zp;
      int zn;
      int xp;
      int xn;
      int zpS;
      for(int i; i <400; i++ ){
        z[1] = myIMU.readFloatAccelZ(); // array of Z accelration at i and i+1 ms
        x[1] = myIMU.readFloatAccelX(); // array of X accelration at i and i+1 ms
        if(z[1] < -6 && z[0] > -6){ // Exceed -6 or not
          zn = zn + 1;
          }
        if(z[1] > 6 && z[0] < 6){ // Exceed 6 or not
          zp = zp + 1;
          }  
        if(x[1] < -6 && x[0] > -6){
          xn = xn + 1;
          }
        if(x[1] > 6 && x[0] < 6){
          xp = xp + 1;
          }
        if(z[1] > 3 && z[0] < 3){
          zpS = zpS + 1;
          }
        z[0] = z[1];
        x[0] = x[1];
        }
    if(zpS >= 2 && xn ==0 && zn ==0){ // If Z acceleration exceeds positive 3 more than twice
      Serial.print("Donble tap!*!*");   //and both z acceleration and x acceleration does not exceed negative 6, print "double tap"
      Serial.print('\n');
      }
    if((xp >= 1 || zp >= 1)&&(xn >= 1 || zn >= 1)){  // If either x or z acceleration exceeds Positive and Negative 6, print Flick
      Serial.print("!!FLICK!!");
      Serial.print('\n');
      }
    }
    
  // Poke ------------------------------------------------------------------------------------------------    
  else if( myIMU.readFloatGyroY() > -300 && myIMU.readFloatGyroY() < 300 && myIMU.readFloatAccelX() > 2 && myIMU.readFloatAccelZ() < 1 && myIMU.readFloatAccelZ() > -1 ){
    // If X Acceleration exceeds 2 and Y gyro reading stays inbetween 300 and -300 and Z acceleration stays in between 1 and -1, excecute a loop for 500 ms
    int i =0;
    float a[2];
    int num;
      
    for (int i; i <500; i++ ){
      a[1] = myIMU.readFloatAccelX();
      if((a[1] > 2 && a[0] < 2) || (a[1] < -2 && a[0] > -2)){ // Count how often it exceed 2 and -2
        num = num + 1;
        }
      a[0]=a[1];
      }
    if(num == 2 || num == 3){ // if it exceeds 2 and -2 at least once in 500ms, print "poke"
      Serial.print("Poke");
      Serial.print('\n');
      }
  }

   // Wave ------------------------------------------------------------------------------------------------ 

   else if((myIMU.readFloatGyroX() > 300 && myIMU.readFloatGyroZ() > 300) || (myIMU.readFloatGyroX() < -300 && myIMU.readFloatGyroZ() < -300)){
    int i = 0;
    float x[2];
    float z[2];
    int numX;
    int numZ;
    for (int i; i <500; i++){
      x[1] = myIMU.readFloatGyroX();
      z[1] = myIMU.readFloatGyroZ();
      if((x[1] > 300 && x[0] < 300)|| (x[1] < -300 && x[0] > -300)){
        numX = numX + 1;
      }
      if((z[1] > 300 && z[0] < 300) || (z[1] < -300 && z[0] > -300)){
        numZ = numZ + 1;
      }
      x[0]=x[1];
      z[0]=z[1];
    }
    if(numX > 3 && numZ > 3){
      Serial.print("Wave");
      Serial.print('\n');
      delay(500);
      }
    } 


    // punch --------------------------------------------------------------------------------------------------

   if( myIMU.readFloatAccelZ() > 3 && myIMU.readFloatAccelX() < 3 && myIMU.readFloatAccelY()< 3 ){ // If Z acceleration exceeds 3 while X and Y acceleration stays below 3, exacute a loop
    int i =0;
    float a[2];
    int num;
      
    for(int i; i <500; i++ ){
      a[1] = myIMU.readFloatAccelZ();
      if (a[1] < 3 && a[0] > 3){
        num = num + 1;
      }
      a[0]=a[1];
    }
    if(num == 1){ // If Z acceleration exceeds 3 once in 5 milliseconds, print "Punch"
      Serial.print("Punch");
      Serial.print('\n');
    }
  }
    
    
//  if (myIMU.readTempC() > 23){
//    Serial.print("TEMP too HIGH");
//    Serial.print('\n');
//  }
   







}
