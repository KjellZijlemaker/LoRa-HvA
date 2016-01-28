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

int e;
int packetCounter = -1;
struct tm* timeinfo;
std::ofstream myfile;

char message1 [] = "BW_125";
//char message2 [] = "PIETTEERRRR";

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: send packets with ACK and retries\n");
  
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
  e = sx1272.setNodeAddress(3);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  printf("SX1272 successfully configured\n\n");
  delay(1000);
}

//Check if the receiver is online before logging
int checkIfOnline(){
        char checkOnlineMessage[] = "CHECK_ONLINE";
	e = sx1272.sendPacketTimeoutACK(8, checkOnlineMessage);
	 if(e == 0){
		cout << "Receiver Found, now Testing";
		return 0;
   	}
	else{
		return 1;
	}
}

// Sending new SF value for receiver
void sendNewSFValue(string sf_value){
   char messageToSend[250];
   strcpy(messageToSend, sf_value.c_str());
   e = sx1272.sendPacketTimeoutACK(8, messageToSend);
}

char* timestamp()
{
   time_t rawtime; 
   time (&rawtime);
   timeinfo = localtime (&rawtime);
   char* time = asctime(timeinfo);
   return time;
}

void loop(string sf_value)
{
   char messageToSend[250];
   strcpy(messageToSend, sf_value.c_str());
   char* time = timestamp();
    // Send packet and print the result
    e = sx1272.sendPacketTimeoutACK(8, messageToSend);
    printf("Packet sent, state %d\n",e);
    
	packetCounter++; //Counter for packet total

	// Making timestamp to txt file
    std::string str1 = "Packet: " + std::to_string(packetCounter) + " BW value: " + sf_value + "\n";
    std::string str2 = std::string(time);
    std::string str3 = str1 + str2;
    myfile.open("logTRANBW.txt",std::ofstream::out | std::ofstream::app);
    myfile <<  str3;
    myfile.close();

   if (e == 0){
	char* time = timestamp();
    	myfile.open("logTRANBW.txt",std::ofstream::out | std::ofstream::app);
    	myfile <<  "ACK from packet: " + std::to_string(packetCounter) + " send at: " + std::string(time) + "\n";
    	myfile.close();
   }
    
//?????????????????????????????
    delay(1000);
}

// Timer for looping through the SF values
void runSFTimer ( int seconds, string sf_value )
{
   clock_t endwait;
   endwait = clock () + seconds * CLOCKS_PER_SEC ;
   while (clock() < endwait)
   {
      loop(sf_value);
   }
   //return 0;
   cout << sf_value << " tests done";
}


int main (){
	setup();
	sx1272.setBW(125);

//	int i = checkIfOnline();
//	while (i != 0){
//		i = checkIfOnline();
//	}
//	if(i == 0){
	runSFTimer(180, "BW_125");
	sendNewSFValue("NEW_BW_250");
	
	sx1272.setBW(250);
	runSFTimer(180, "BW_250");
	sendNewSFValue("NEW_BW_500");

	sx1272.setBW(500);
   	runSFTimer(180, "BW_500");

	printf("BW test done");
	return (0);
//	}
}
