/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.1
 *  Design:            David Gascón 
 *  Implementation:    Covadonga Albiñana & Victor Boria
 */
 
// Include the SX1272 and SPI library: 
#include "SX1272.h"
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
using namespace std;

int packetCounter = 0;
int e;
char my_packet[100];
time_t start;
time_t ending;
double elapsed;

FILE *f;
time_t rawtime;

struct tm * timeinfo;
std::ofstream myfile;

int loop_count = 1;

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets with ACK and retries\n");
  
  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);
  
  // Set transmission mode
  e = sx1272.setMode(4);
  printf("Setting Mode: state %d\n", e);
  
  // Set header
  e = sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);
  
  // Select frequency channel
  e = sx1272.setChannel(CH_10_868);
  printf("Setting Channel: state %d\n", e);
  
  // Set CRC
  e = sx1272.setCRC_ON();
  printf("Setting CRC ON: state %d\n", e);
  
  // Select output power (Max, High or Low)
  e = sx1272.setPower('L');
  printf("Setting Power: state %d\n", e);
  
  // Set the node address
  e = sx1272.setNodeAddress(8);
  printf("Setting Node address: state %d\n", e);
  


  // Print a success message
  printf("SX1272 successfully configured\n\n");
  delay(1000);
}





void loop(void)
{ 
  //char* m = "Packet:";  
 
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  
  char* time_message = asctime(timeinfo);
  packetCounter++;    
  
    if( loop_count == 1 ){


       time(&start);


       loop_count = 0;
    }



    std::string str1 = "Packet: " + std::to_string(packetCounter) + " ";
    std::string str2 = std::string(time_message);
    std::string str3 = str1 + str2;
    myfile.open("log.txt",std::ofstream::out | std::ofstream::app);
    myfile <<  str3;
    myfile.close();

    do {
        time(&ending);
        elapsed = difftime(ending, start);
        /* For most data types one could simply use
            elapsed = end - start;
            but for time_t one needs to use difftime(). */

        printf("Time elapsed: &#37;f\n", elapsed);
    } while(elapsed < 10);



  // Receive message
  e = sx1272.receivePacketTimeoutACK(10000);
  if ( e == 0 )
  {

    if( loop_count == 1 ){
       
 
       time(&start); 
       
       
       loop_count = 0;
    }


    printf("Receive packet with ACK and retries, state %d\n",e);
    

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];     
      
    }
    printf("Message: %s\n", my_packet);
  
    std::string str1 = "Packet: " + std::to_string(packetCounter) + " ";
    std::string str2 = std::string(time_message);
    std::string str3 = str1 + str2;
    myfile.open("log.txt",std::ofstream::out | std::ofstream::app);
    myfile <<  str3;
    myfile.close();
    
    do {
        time(&ending);
        elapsed = difftime(ending, start);
        /* For most data types one could simply use
            elapsed = end - start;
            but for time_t one needs to use difftime(). */
 

        printf("Time elapsed: &#37;f\n", elapsed);
    } while(elapsed < 10);

 
    int i =  sx1272.getSNR();
    int j =  sx1272.getRSSI();
    int l =  sx1272.getRSSIpacket();
    //int f = sx1272.getMode();
  //  int j = sx1272.getPreambleLength();
//    int l = sx1272.getPacketMAXTimeout();
 



  }
  else {
    printf("Receive packet with ACK and retries, state %d\n",e);
  }
}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}