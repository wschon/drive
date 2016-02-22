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



#include <SPI.h>
#include "mcp_can.h"


unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
bool fwd = false;
bool rev = false;


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
          switch(buf[len]) {          //The last bit on the message holds the specificdriving command
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
              else {                    //


          }
        }



            Serial.print(buf[i]);Serial.print("\t");
        }
        Serial.println();
        //delay(1000);
    }
}
//pulse function to send pulses to reach a certain speed when moving forwards/backwards

void pulse(double on, double off){
    int n=3;
    if (fwd)
    int n = 2;
    digitalWrite(n, HIGH);
    delayMicroseconds(on);
    digitalWrite(n, LOW);
    delayMicroseconds(off);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
