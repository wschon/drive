/*Arduino code for motor control
0 0 0 0 0
0 0 0 0 1
0 0 0 1 0
0 0 1 0 0
0 1 0 0 0
1 0 0 0 0
and drives pins correspondingly to drive wheelchair via 5-way controller
Wesley Schon, Dan Pettinger
Georgia Tech VIP Secure Hardware 2016
*/
/*NEW FORMAT FOR CAN BUS MESSAGES:
 * 3 0 0 0 x
 * ^3 indicates that this is a drive command
 *
 * x can be replaced with various numbers to indicate specific command:
 * 0 = brake
 * 1 = forward
 * 2 = reverse
 * 3 = right
 * 4 = left
 *
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
int on = 200; //arbitrary numbers for frequency of pulses, needed to be changed.
int off = 200;
int n = 20;


MCP_CAN CAN(10);                                            // Set CS to pin 10

void setup()
{
    Serial.begin(115200);
     for (int i=0; i<=4; i++) {;
      pinMode(i+2, OUTPUT);
     }

START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
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

        ///////////Commenting out the next few lines for now
/*
        for(int i = 0; i<len; i++)  {  // print the data
        if (buf[i] == 1)
        digitalWrite(i+2,HIGH);
        else
        digitalWrite(i+2,LOW);
        //Serial.println("Stuff happening");
*/


if (buf[0] == 3 {             //if the message on the CAN bus is a driving command
  switch(buf[len-1]) {          //The last bit on the message holds the specificdriving command, last bit should be (len-1),right?
    case 0:                   //brake case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      fwd = false;
      rev = false;
    case 1:                    //Forward case
      if (fwd == true)  {      //if the wheelchair was already driving forward
        digitalWrite(2, HIGH); //Wheelchair was already driving forward and then adjusted.  Do not re-pulse, just drive all pins high
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
      }
      else {                    //The wheelchair needs to start forward from stationary by pulsing input for several seconds...
        /////INSERT FUNCTION CALL OR LOOP HERE!!!!
        pulse(on,off);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
      }
    case 2:                     //Reverse case
      //////INSERT FUNCTION CALL HERE!!!
      fwd = false; //specify the moving direction if not set before. 
      rev = true;
      pulse(on,off);  
    case 3:                    //Turning right case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
    case 4:                    //Turning left case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);



  }
}



            Serial.print(buf[i]);Serial.print("\t");
        }
        Serial.println();
        //delay(1000);
    }
}
//pulse function to send pulses to reach a certain speed when moving forwards/backwards
void pulse(double on, double off,int n){
    int n=3;
    if (fwd)
    n = 2;
    for (int i =0; i<n;i++){
    digitalWrite(n, HIGH);
    delayMicroseconds(on);
    digitalWrite(n, LOW);
    delayMicroseconds(off);
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
