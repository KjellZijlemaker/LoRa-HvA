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

int e;

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
   }
    

    delay(4000);
 
 	// Send message2 broadcast and print the result
    e = sx1272.sendPacketTimeoutACKRetries(0, message2);
    printf("Packet sent, state %d\n",e);
    
    delay(4000);
}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
