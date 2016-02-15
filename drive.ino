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

        for(int i = 0; i<len; i++)  {  // print the data
        if (buf[i] == 1) 
        digitalWrite(i+2,HIGH);
        else
        digitalWrite(i+2,LOW);
        //Serial.println("Stuff happening");
        
        
            Serial.print(buf[i]);Serial.print("\t");
        }
        Serial.println();
        //delay(1000);
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
