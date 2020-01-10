/**
 * File: NetworkCraneWebsite
 * Coder: DaHuntr9
 * File Description:
 * The following code is a maintenance of previous FGCU students Crane Project to fit a new networking project,
 * which allows for the control of the crane from a externally hosted webpage. 
 * 
 * Network Notes:
 * Please contact the professor who assigned you to the project to ensure that you are using the correct network information.
 * Please do not commit code to version control with loaded ip addresses as it opens up the devices to external access.  
 * In order for this project to be implemented. 
 * 
 * 
 * Hardware Notes: 
 * As of November 2019 there are two continuous solenoids on the crane, these solenoids act diffrent from classical solenoids. 
 * When the .write() function is used for continuous solenoids it sets the speed of the solenoids. 92 seems to be 0 velocity. 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

// Setting Servo Objects to apply commands. 
Servo winchServo;
Servo boomServo;

// Setting the pins for the Arduino to control. 
int magnetController = 9; // Pin 9 associated with the transistor. 
int magnetLED = 7;        // Pin 7 associated with the LED. 
int boomPin = 5;          // Pin 5 for boom Servo.
int winchPin = 6;         // Pin 5 for winch Servo.
String readString;        // Set up space for Input from server to be read to. 

// Associated Data for Ethernet Connectivity - These are to be set by the individual user depending on the network. 
EthernetServer server();          // Set server port
byte mac[] = {};                  // Physical MAC Address (Set by coder)
byte ip[] = {};                   // Local IP Address set by router. 
byte subnet_mask[] = {};
//byte dns[] = {};
byte gate_way[]={};


//Setup Function for Arduino
void setup()
{
  // Open serial (USB) communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

  // Binding servos to associated pins. 
  winchServo.attach(winchPin);
  boomServo.attach(boomPin);

  // Binding devices to associated pins. 
  pinMode(magnetLED, OUTPUT);
  pinMode(magnetController, OUTPUT);
  
  // Start the Ethernet connection and the Server, It will display on the serial connection. 
  Ethernet.begin(mac, ip, gate_way, subnet_mask);
  server.begin();
  Serial.print("Server is at :");
  Serial.println(Ethernet.localIP());
  Serial.print("gatewayip is at :");
  Serial.println(Ethernet.gatewayIP());
  Serial.print(" dns server is at :");
  Serial.print(Ethernet.dnsServerIP());

  // Upon initally receiving power both servos will drift until set to 92. 
  boomServo.write(92);  // 0% speed
  winchServo.write(92); // 0% speed
}

void loop()
{ 
  // Create a Client connection.
  EthernetClient client = server.available();
  if (client)
  {
    if (client.available())
    {
      char c = client.read();
      //read char by char HTTP request
      if (readString.length() < 100)
      {
        //store characters to string
        readString += c;
        Serial.print(c);
      }
      //if HTTP request has ended
      if (c == '\n')
      {
        Serial.println(readString);        //print to serial monitor for debuging Web Server Code
        client.println("HTTP/1.1 200 OK"); //send new page
        client.println("Content-Type: text/html");
        client.println();
        client.println("<HTML>");
        client.println("<HEAD>");
        client.println("<CENTER>");
        client.println("<TITLE>Maintence Project</TITLE>");
        client.println("</HEAD>");
        client.println("<BODY>");
        client.println("<hr />");
        client.println("<br />");
        client.println("<H2>Arduino with Ethernet Shield</H2>");
        client.println("<br />");
        client.println("<H2>To operate this crane the spindle must be attached to servo</H2>");
        client.println("<H2>The spindle will move according to how the thread it is spun on it. BE CAREFUL</H2>");
        client.println("<H2>To Move between the two cups push left or right.</H2>");
        client.println("<br />");
        client.println("<br />");
        client.println("<a href=\"/?buttonRight\"\">Turn Right</a>");
        client.println("<a href=\"/?buttonLeft\"\">Turn Left</a><br />");
        client.println("<br />");
        client.println("<br />");
        client.println("<a href=\"/?buttonUp\"\">Rotate Up</a>");
        client.println("<a href=\"/?buttonDown\"\">Rotate Down</a><br />");
        client.println("<br />");
        client.println("<a href=\"/?buttonMagnetOn\"\">Magnet On</a>");
        client.println("<a href=\"/?buttonMagnetOff\"\">Magnet Off</a><br />");
        client.println("<br />");
        client.println("<br />");
        client.println("<a href=\"/?buttonStop\"\">Stop Motion</a>");
        client.println("<br />");
        client.println("</CENTER>");
        client.println("</BODY>");
        client.println("</HTML>");
        delay(1);
        //stopping client
        client.stop();
        //controls the Arduino if you press the buttons Web Server Button Control

        // Upon the "Turn Right" button being pressed, It sets the continuous boom servo to move right. 
        if (readString.indexOf("?buttonRight") > 0)
        {
          Serial.print("Boom: Moving Right");
          //Sets the speed to 20%~ Right
          boomServo.write(100);
        }

        // Upon the "Turn Left" button being pressed, It sets the continuous boom servo to move Left. 
        if (readString.indexOf("?buttonLeft") > 0)
        {
          Serial.print("Boom: Moving Left");
          //Sets the speed to 20%~ Left
          boomServo.write(80);
        }
        
        // Upon the "Rotate Up" button being pressed, It sets the continuous winch servo to move up. 
        if (readString.indexOf("?buttonUp") > 0)
        {
          Serial.print("Winch: Moving Up");
          //Sets speed to 20%~ upwards
          winchServo.write(180);
        }
        
        // Upon the "Rotate Down" button being pressed, It sets the continuous winch servo to move down. 
        if (readString.indexOf("?buttonDown") > 0)
        {
          Serial.print("Winch: Moving Down");
          //Sets speed to 20%~ downwards
          winchServo.write(0);
        }
        
        // Upon the "Magnet On" button being pressed, It sets the breaker to on and sends current to the solenoid.
        if (readString.indexOf("?buttonMagnetOn") > 0)
        {
          Serial.print("Magnet:On");
          digitalWrite(magnetLED, HIGH);        //Sets LED pin to HIGH, turning it on. 
          digitalWrite(magnetController, HIGH); //Sets Controller pin to HIGH, turning it on. 
        }
        
        // Upon the "Magnet Off" button being pressed, It sets the breaker to off and prevents current from reaching the solenoid.
        if (readString.indexOf("?buttonMagnetOff") > 0)
        {
          Serial.print("Magnet:Off");
          digitalWrite(magnetLED, LOW);         //Sets LED pin to LOW, turning it off.
          digitalWrite(magnetController, LOW);  //Sets Controller pin to LOW, turning it off.
        }

        //Upon the "Stop Motion" button being pressed, It stops both solenoids. 
        if (readString.indexOf("?buttonStop") > 0)
        {
          boomServo.write(92);  // 0% speed
          winchServo.write(92); // 0% speed
        }

        //clearing string for next read
        readString = "";
      }
    }
  }
}
