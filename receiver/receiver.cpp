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
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
using namespace std;

string Received_message = "";
int packetCounter = 0;
int e;
char my_packet[100];
FILE *f;
time_t rawtime;
struct tm * timeinfo;
std::ofstream myfile;


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

void checkMessage(string sf_value){
const char* value = sf_value.c_str();
//printf(value);

if(strcmp(value, "SF_6") == 0){
printf("To 6!");  
sx1272.setSF(SF_6);
}

if(strcmp(value, "SF_7") == 0){
sx1272.setSF(SF_7);
}

if(strcmp(value, "SF_8") == 0){
sx1272.setSF(SF_8);
}

if(strcmp(value, "SF_9") == 0){
sx1272.setSF(SF_9);
}

if(strcmp(value, "SF_10") == 0){
sx1272.setSF(SF_10);
}

if(strcmp(value, "SF_11") == 0){
sx1272.setSF(SF_11);
}

if(strcmp(value, "SF_12") == 0){
printf("To 12!");
sx1272.setSF(SF_12);
}
}

void loop(void)
{ 
  // Receive message
  e = sx1272.receivePacketTimeoutACK(10000);

  if ( e == 0 ){

    printf("Receive packet with ACK and retries, state %d\n", e);
    packetCounter++; 

	for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
	{
		my_packet[i] = (char)sx1272.packet_received.data[i];
	}
	//Writing message, packet counter and received timestamp to file.
	
    string message1 = "Message: ";
    string message2 (my_packet);
    string Received_message = message1 = message2;
	
    time (&rawtime);
    timeinfo = localtime (&rawtime); 
    char* time = asctime(timeinfo);
	
    std::string str1 = "Packet: " + std::to_string(packetCounter) + " ";
    std::string str2 = std::string(time);
    std::string str3 = str1 + str2;
    myfile.open("log.txt",std::ofstream::out | std::ofstream::app);
    myfile <<  str3;
	myfile <<  Received_message;
	myfile << "\n";
	myfile <<  endl;
    myfile.close();

   checkMessage(message2);
  }
  else {
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	char* time = asctime(timeinfo);
	myfile.open("log.txt",std::ofstream::out | std::ofstream::app);
        myfile <<  "No Packet received at: " + std::string(time); 
        myfile.close();
        printf("No packet received, state %d\n", e);
  }
}

int main (){
	setup();
	sx1272.setSF(SF_7);
//	sx1272.setBW(BW_250);
//	sx1272.setCR(CR_7);
//	sx1272.setSF(SF_9);
	//clock_t endwait;
	//endwait = clock() + 300 * CLOCKS_PER_SEC;
	//while (clock() < endwait)
	//{
	while(true){
		loop();
	}
	//}
	return (0);
}
