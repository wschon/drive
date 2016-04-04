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
 * 6 = right                p4
 * 7 = left                 p5
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
int on = 500; //arbitrary numbers for frequency of pulses, need to be changed.
int off = 500;
int cycles = 5;
int on2 = 500;
int off2 = 500;
int cycles2 = 2;
int reverse_count = 0;
int pulsecount = 0;
bool inc = true;
int per1 = 8000;
int per2 = 1200;


MCP_CAN CAN(10);                                            // Set CS to pin 10

void setup()
{
    Serial.begin(9600);
     for (int i=0; i<=4; i++) {;
      pinMode(i+2, OUTPUT);
     }

START_INIT:

    if(CAN_OK == CAN.begin(CAN_1000KBPS))                   // init can bus : baudrate = 1000k
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
    pulse(2, per1, &pulsecount, 20); //50% duty cycle
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    break;

    case 3:                    //Forward case
        digitalWrite(2, HIGH); //Wheelchair was already driving forward and then adjusted.  Do not re-pulse, just drive all pins high
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        break;

    case 4:             //Accelerate reverse case

        digitalWrite(2, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        pulse(3, per2, &pulsecount, 15);
        if (reverse_count >= 10 & per2 >= 50) {
          per2-=10;
          reverse_count = 0;
          Serial.print("Period 2:");
          Serial.print(per2);
          Serial.println();
        }
          reverse_count++;
        break;
    case 5:                     //Reverse case

      digitalWrite(2, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      pulse(3, per2, &pulsecount, 20);
      break;

    case 7:                    //Turning left case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      break;
    case 6:                    //Turning right case
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      break;

}
//            Serial.print(buf[i]);Serial.print("\t");
        }
        Serial.println();
        //delay(1000);
    }
}
//pulse function to send pulses to reach a certain speed when moving forwards/backwards
void pulse(int pin, int per, int* pulsecount, int dc){

    if (inc) {
      *pulsecount++;
      Serial.print(*pulsecount);
      Serial.print("\t");
      digitalWrite(pin, LOW);
      if (*pulsecount >= per * dc / 100) {
        Serial.print("Toggle inc to false");
        inc = false;
        *pulsecount = *pulsecount * (100-dc) / 100;
      }
      }
    else {
       *pulsecount--;
       digitalWrite(pin, HIGH);
       if(*pulsecount <= 0) {
        Serial.println("Toggle inc to true");
        inc = true;
       }
    }


}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
