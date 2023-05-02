/* @file HelloKeypad.pde|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library for the Velleman VMA300.
|| #
*/
float angle = 0;
float distance = 999;
long duration;
float distancecm;
int leftMotor_speed, rightMotor_speed, servoAngle;
int i = 0;
int j = 0;
const int wheelRadius = 6;


#include <math.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>


#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal
int x = 0;
const int MAX_COMMANDS = 100;
char thething[MAX_COMMANDS][2]; // thething to store directional commands

const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPins[ROWS] = {18, 5, 17, 16}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 0, 2};       // connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD SCREEN STUFF

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  



// make some custom characters:
byte Heart[8] = {
0b00000,
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b00000,
0b00000
};

byte Bell[8] = {
0b00100,
0b01110,
0b01110,
0b01110,
0b11111,
0b00000,
0b00100,
0b00000
};


byte Alien[8] = {
0b11111,
0b10101,
0b11111,
0b11111,
0b01110,
0b01010,
0b11011,
0b00000
};

byte Check[8] = {
0b00000,
0b00001,
0b00011,
0b10110,
0b11100,
0b01000,
0b00000,
0b00000
};

byte Speaker[8] = {
0b00001,
0b00011,
0b01111,
0b01111,
0b01111,
0b00011,
0b00001,
0b00000
};


byte Sound[8] = {
0b00001,
0b00011,
0b00101,
0b01001,
0b01001,
0b01011,
0b11011,
0b11000
};


byte Skull[8] = {
0b00000,
0b01110,
0b10101,
0b11011,
0b01110,
0b01110,
0b00000,
0b00000
};

byte Lock[8] = {
0b01110,
0b10001,
0b10001,
0b11111,
0b11011,
0b11011,
0b11111,
0b00000
};


// END OF LCD SCREEN STUFF /////




void setup()
{
// KEYPAD
  Wire.begin();
  Serial.begin(9600);

// LCD SCREEN
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // create a new characters
  lcd.createChar(0, Heart);

  // create a new characters
  lcd.createChar(1, Speaker);



              // set cursor to first column, first row
    lcd.setCursor(0, 0);
            // print message
    lcd.print("1=F");

    lcd.setCursor(0, 1);
            // print message
    lcd.print("2=B");

    lcd.setCursor(5, 0);
            // print message
    lcd.print("3=R");

    lcd.setCursor(5, 1);
            // print message
    lcd.print("4=L");

    lcd.setCursor(9, 0);
            // print message
    lcd.print("5=Stop");

    lcd.setCursor(9, 1);
            // print message
    lcd.print("#=Start");

}


void Transmit_to_arduino(int leftMotor_speed, int rightMotor_speed, int servoAngle)
{
    Wire.beginTransmission(I2C_SLAVE_ADDR);                   // transmit to device #4
    Wire.write((byte)((leftMotor_speed & 0x0000FF00) >> 8));  // first byte of x, containing bits 16 to 9
    Wire.write((byte)(leftMotor_speed & 0x000000FF));         // second byte of x, containing the 8 LSB - bits 8 to 1
    Wire.write((byte)((rightMotor_speed & 0x0000FF00) >> 8)); // first byte of y, containing bits 16 to 9
    Wire.write((byte)(rightMotor_speed & 0x000000FF));        // second byte of y, containing the 8 LSB - bits 8 to 1
    Wire.write((byte)((servoAngle & 0x0000FF00) >> 8));       // first byte of x, containing bits 16 to 9
    Wire.write((byte)(servoAngle & 0x000000FF));
    Wire.endTransmission(); // stop transmitting
  

}

void loop()
{

//ENCODER    
    // initialise distance variable
    float ENdistance = 0;
    



  // // send a request for data to the slave device
  // Wire.requestFrom(I2C_SLAVE_ADDR, 2); // request 2 bytes of data

  // // wait for response
  // while (Wire.available() < 2) {
  //   // do nothing
  // }

  // // read response data
  // int enc1_count = Wire.read();
  // int enc2_count = Wire.read();

  // // print the received data to the serial monitor
  // // Serial.print("Enc1 count: ");
  // // Serial.println(enc1_count);
  // // Serial.print("Enc2 count: ");
  // // Serial.println(enc2_count);

  // // delay before sending the next request
  // delay(500);
















//KEYPAD
    int key = keypad.getKey();
    if (key) // on button press
    {
      lcd.clear();

    
      
        if (key == '#') // if input = 0 then execute these commands
        {

            for (int m = 0; m <= i; m++)
            {
                int n = 0;
                Serial.println(thething[m][n]);
              
                if (thething[m][n] == '1') // forward
                {                 
                    leftMotor_speed = -155;
                    rightMotor_speed = 155;
                    servoAngle = 93;
                    Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle);
                    n = 1;

                    lcd.clear();
                    lcd.noBlink();                    
                    lcd.setCursor(0, 0);
                    lcd.print("Moving Forward");

                    // send a request for data to the slave device
                    char wagwan=(thething[m][n]);
                    int ia = wagwan - '0';                    

                      
                    while (ENdistance < (ia*125)) { 
                        Serial.print(wagwan);                        
                        Wire.requestFrom(I2C_SLAVE_ADDR, 2); // request 2 bytes of data

                        // wait for response
                        //while (Wire.available() < 2) {
                              // do nothing
                              //Serial.print("help");                              
                        //}

                          // read response data
                        int enc1_count = Wire.read();
                        int enc2_count = Wire.read();
                      
                        //Serial.print(enc1_count);
                        //Serial.print("\n");
                        //Serial.print(enc2_count);
                        //Serial.print("\n");
                        long encoderCount = enc1_count; // read the encoder count
                        ENdistance += (encoderCount * wheelRadius * PI) / 360.0; // calculate the distance travelled in cm
                        Serial.print("Distance: ");
                        Serial.println(ENdistance);

                        /*Serial.print(enc1_count);
                        Serial.print("\n");
                        Serial.print(enc2_count);
                        Serial.print("\n");*/

                        

                        delay(1);
                    }
                    ENdistance=0;                    
                    // delay((thething[m][n]) * 19);
                    lcd.clear();
                    
                }
                else if (thething[m][n] == '2') // backward
                {
                    leftMotor_speed = 155;
                    rightMotor_speed = -155;
                    servoAngle = 93;
                    Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle);
                    n = 1;
                    lcd.noBlink();
                    lcd.setCursor(0, 0);
                    lcd.print("Moving Back");
                    
                    delay((thething[m][n]) * 19);
                    lcd.clear();
                }
                else if (thething[m][n] == '3') // left
                {
                    leftMotor_speed = -155;
                    rightMotor_speed = 255;
                    servoAngle = 48;
                    Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle);
                    n = 1;
                    lcd.noBlink();
                    lcd.setCursor(0, 0);
                    lcd.print("Turning Left");
              
                    delay((thething[m][n]) * 12);
                    lcd.clear();
                }
                else if (thething[m][n] == '4') // right
                {
                    leftMotor_speed = -255;
                    rightMotor_speed = 155;
                    servoAngle = 138;
                    Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle);
                    n = 1;
                    lcd.noBlink();
                    lcd.setCursor(0, 0);
                    lcd.print("Turning Right");
              
                    delay((thething[m][n]) * 12);
                    lcd.clear();
                }
                else if (thething[m][n] == '5') // stop
                {
                    leftMotor_speed = 0;
                    rightMotor_speed = 0;
                    servoAngle = 93;
                    Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle);
                    n = 1;
                  
                    lcd.noBlink();
                    lcd.setCursor(0, 0);
                    lcd.print("Stopped");
              
                    delay((thething[m][n]) * 19);
                    lcd.clear();
                }
                // Serial.println(m);
                n = 0;
            }
            memset(thething, 0, sizeof(thething));
            leftMotor_speed = 0;
            rightMotor_speed = 0;
            servoAngle = 93;
            Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle);
            i = 0;
            j = 0;
            

        }

        else if (key == '*') 
        {
          // clear the thething
          for (int h = 0; h < MAX_COMMANDS; h++) {
            thething[h][0] = '\0';
            thething[h][1] = '\0';
          }
          i = 0;
          j = 0;

        
        }
        else // if input != #, keep building up commands
        {
            thething[i][j] = key;
            // Serial.print("thething");
            // Serial.print("[");
            // Serial.print(i);
            // Serial.print(",");
            // Serial.print(j);
            // Serial.print("]");
            // Serial.print(": ");
            // Serial.println(thething[i][j]);
            j = j + 1;  // increment j


            // set cursor to first column, first row
            lcd.setCursor(0, 0);
            // print message
            lcd.print("1=F");

            lcd.setCursor(0, 1);
            // print message
            lcd.print("2=B");

            lcd.setCursor(5, 0);
            // print message
            lcd.print("3=R");

            lcd.setCursor(5, 1);
            // print message
            lcd.print("4=L");

            lcd.setCursor(9, 0);
            // print message
            lcd.print("5=Stop");

            lcd.setCursor(9, 1);
            // print message
            lcd.print("#=Start");

            

        
            if (j == 2) // keep j equal to 0 or 1
            {
                j = 0;
                i = i + 1; // increment i
            }
            if (i == MAX_COMMANDS) // keep i less than maximum commands
            {
                i = 0;
            }



            if (j == 1)
            {
              lcd.clear();
              lcd.setCursor(0, 0);
              // print message
              lcd.print("Enter Delay:");
      
            }

         

            //delay(200);
        }
    }
}

// void encoderDistance(){
//   long encoderCount = encoder.read();
//   float distance = (encoderCount * wheelRadius * M_PI) / 360.0;

// }*/