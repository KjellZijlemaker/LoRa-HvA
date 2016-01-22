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
int packetCounter = 0;

char message1 [] = "HET WERKKTTT HERRRR PIETERR";
char message2 [] = "PIETTEERRRR";

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

void loop(void)
{
    // Send message1 and print the result
    e = sx1272.sendPacketTimeoutACKRetries(8, message1);
    int f = sx1272.getMode();
    printf("Packet sent, state %d\n",e);
    
   if(e == 0){
	//printf("Mode is: " + sx1272._mode);
	packetCounter++; 
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	char* time = asctime(timeinfo);
	
    std::string str1 = "Packet: " + std::to_string(packetCounter) + " ";
    std::string str2 = std::string(time);
    std::string str3 = str1 + str2;
    myfile.open("log.txt",std::ofstream::out | std::ofstream::app);
    myfile <<  str3;
	myfile <<  endl;
    myfile.close();
   }
    

    delay(4000);
 
 	// Send message2 broadcast and print the result
    e = sx1272.sendPacketTimeoutACKRetries(0, message2);
    printf("Packet sent, state %d\n",e);
    
    delay(4000);
	
	
}

int main (){
	setup();
        FILE *f;
	sx1272.setSF(SF_6);
	clock_t t;
	clock_t u;
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF 6 tests done, moving to SF 7 tests");
	sx1272.setSF(SF_7);
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF 7 tests done, moving to SF 8 tests");
	sx1272.setSF(SF_8);
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF 8 tests done, moving to SF 9 tests");
	sx1272.setSF(SF_9);
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF 9 tests done, moving to SF 10 tests");
	sx1272.setSF(SF_10);
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF 10 tests done, moving to SF 11 tests");
	sx1272.setSF(SF_11);
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF 11 tests done, moving to SF 12 tests");
	sx1272.setSF(SF_12);
	t = clock();
	while((u = clock() -t) <= 300000){
		loop();
	}
	printf("SF test done");
	return (0);
}
