/*Arduino code for motor control
0 0 0 0 0
0 0 0 0 1
0 0 0 1 0
0 0 1 0 0
0 1 0 0 0
1 0 0 0 0
and drives pins correspondingly to drive wheelchair via 5-way controller
Wesley Schon, Dan Pettinger, Siyan Yu
Georgia Tech VIP Secure Hardware and Robosense 2016
*/
/*NEW FORMAT FOR CAN BUS MESSAGES:
 * 3 0 0 0 x
 * ^3 indicates that this is a drive command
 *
 * x can be replaced with various numbers to indicate specific command:

 * 1 = brake                p6
 * 2 = accelerate forward   p2
 * 3 = forward              p2
 * 4 = accelerate reverse   p3
 * 5 = reverse              p3
 * 6 = left                 p4
 * 7 = right                p5
 * middle 3 bits are arbitrary.  They could potentially be used to include other information along with
 * driving commands on the bus simultaneously in the future (since this code ignores them)
 */



#include <SPI.h>
#include "mcp_can.h"


unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
bool fwd = false;
bool rev = false;
int on = 200; //arbitrary numbers for frequency of pulses, need to be changed.
int off = 200;
int cycles = 5;


MCP_CAN CAN(10);                                            // Set CS to pin 10

void setup()
{
    Serial.begin(9600);
     for (int i=0; i<=4; i++) {;
      pinMode(i+2, OUTPUT);
     }

START_INIT:

    if(CAN_OK == CAN.begin(CAN_1000KBPS))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}


void loop()
{
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }
        Serial.println();
        ///////////Commenting out the next few lines for now
/*
        for(int i = 0; i<len; i++)  {  // print the data
        if (buf[i] == 1)
        digitalWrite(i+2,HIGH);
        else
        digitalWrite(i+2,LOW);
        //Serial.println("Stuff happening");
*/


if (buf[0] == 3) {             //if the message on the CAN bus is a driving command
  switch(buf[4]) {          //The last bit on the message holds the specificdriving command, last bit should be (len-1),right?
    case 1:                   //brake case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      fwd = false;
      rev = false;
      break;

    case 2:             //Accelerate forward case
    fwd = true;
    rev = false;

    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    pulse(on, off, cycles);

    case 3:                    //Forward case
      if (fwd == true)  {      //if the wheelchair was already driving forward
        digitalWrite(2, HIGH); //Wheelchair was already driving forward and then adjusted.  Do not re-pulse, just drive all pins high
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
      }
      else {                    //The wheelchair needs to start forward from stationary by pulsing input for several seconds...
        /////INSERT FUNCTION CALL OR LOOP HERE!!!!
        fwd = true;
        rev = false;
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        pulse(on,off,cycles);
      }

    case 4:             //Accelerate reverse case
        fwd = false;
        rev = true;
        digitalWrite(2, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        pulse(on, off, cycles);
    case 5:                     //Reverse case
    if (rev == true)  {      //if the wheelchair was already driving forward
        digitalWrite(2, HIGH); //Wheelchair was already driving forward and then adjusted.  Do not re-pulse, just drive all pins high
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
      }
      else {
      //////INSERT FUNCTION CALL HERE!!!
      fwd = false; //specify the moving direction if not set before.
      rev = true;
      digitalWrite(2, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      pulse(on,off,cycles);

    case 6:                    //Turning right case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
    case 7:                    //Turning left case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
  }
}



//            Serial.print(buf[i]);Serial.print("\t");
        }
        Serial.println();
        //delay(1000);
    }
}
//pulse function to send pulses to reach a certain speed when moving forwards/backwards
void pulse(double on, double off,int cycles){
    int n=3;
    if (fwd)
    n = 2;
    for (int i =0; i<=cycles ;i++){
    digitalWrite(n, LOW);
    delayMicroseconds(on);
    digitalWrite(n, HIGH);
    delayMicroseconds(off);
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
