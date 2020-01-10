/**
 * File: NetworkCraneWebsite
 * Coder: DaHuntr9
 * 
 * File Description:
 * The following code is a maintenance of previous FGCU students Crane Project to fit a new networking project,
 * which allows for the control of the crane from a externally hosted webpage. 
 * 
 * Hardware Notes: 
 * As of November 2019 there are two continuous servos on the crane, these servos act diffrent from classical servos. 
 * When the .write() function is used for continuous servos it sets the speed of the servo. 92 seems to be 0 velocity
 * but can change upon multiple restarts. 
 */

#include <SPI.h>
#include <Servo.h>

// Setting Servo Objects to apply commands. 
Servo winchServo;
Servo boomServo;

// Setting the pins for the Arduino to control. 
int magnetController = 9; // Pin 9 associated with the transistor. 
int magnetLED = 7;        // Pin 7 associated with the LED. 
int boomPin = 5;          // Pin 5 for boom Servo.
int winchPin = 6;         // Pin 5 for winch Servo.
String inputBuffer;  // Set up space for Input from server to be read to. 



//Setup Function for Arduino
void setup()
{
  // Open serial (USB) communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }
  Serial.println("Connected");

  // Binding servos to associated pins. 
  winchServo.attach(winchPin);
  boomServo.attach(boomPin);

  // Binding devices to associated pins. 
  pinMode(magnetLED, OUTPUT);
  pinMode(magnetController, OUTPUT);

  // Upon initally receiving power both servos will drift until set to 92. 
  boomServo.write(91);  // 0% speed
  winchServo.write(92); // 0% speed
}

void loop()
{ 
  // Listens for input.
  while(Serial.available() > 0)
  {
      inputBuffer = Serial.readString(); //Creates a array that is 14 in length
      inputBuffer.trim();
      if(inputBuffer != ""){
        Serial.println(inputBuffer);
      }
  }
  
  
  //Conditional logic for Crane movement. 
  // Winch moving up.
  if( inputBuffer.equals("movingZ:up"))
  {
    Serial.println("Winch: Moving Up");
    //Sets speed to 100% upwards
    winchServo.write(180);
  }
  // Winch moving down.
  else if(inputBuffer.equals("movingZ:down"))
  {
    Serial.println("Winch: Moving Down");
    //Sets speed to 80%~ downwards
    winchServo.write(20);
  }
  // Boom moving clockwise
  else if(inputBuffer.equals("movingXY:right"))
  {
    Serial.println("Boom: Moving Right");
    //Sets the speed to 20%~ Right
    boomServo.write(100);
  }
  // Boom moving counterclockwise
  else if(inputBuffer.equals("movingXY:left"))
  {
    Serial.println("Boom: Moving Left");
    //Sets the speed to 20%~ Left
    boomServo.write(80);
  }
  // Magnet Turning On 
  else if(inputBuffer.equals("magnet:on"))
  {
    Serial.println("Magnet:On");
    digitalWrite(magnetLED, HIGH);        //Sets LED pin to HIGH, turning it on. 
    digitalWrite(magnetController, HIGH); //Sets Controller pin to HIGH, turning it on.
  }
  // Magnet Turning Off
  else if(inputBuffer.equals("magnet:off"))
  {
    Serial.println("Magnet:Off");
    digitalWrite(magnetLED, LOW);         //Sets LED pin to LOW, turning it off.
    digitalWrite(magnetController, LOW);  //Sets Controller pin to LOW, turning it off.
  }
  // Stop Crane Motion
  else if(inputBuffer.equals("movingZ:stop"))
  {
    Serial.println("Stopping Winch");
    winchServo.write(92); // 0% speed
  }
  // Stop Crane Motion
  else if(inputBuffer.equals("movingXY:stop"))
  {
    Serial.println("Stopping Boom");
    boomServo.write(91);  // 0% speed
  }
  inputBuffer = "";
}