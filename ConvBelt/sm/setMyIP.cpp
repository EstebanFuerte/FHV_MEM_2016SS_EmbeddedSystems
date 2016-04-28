// Includes for setLocalIp()
#include <stdio.h>
#include <string.h>
#include <ioLib.h>
#include <sockLib.h>
#include <net/if.h>
#include <ipnet/ipioctl.h>
#include "ifLib.h"

char myIPAddress [10];

// Get local MAC address and set correct IP
void setLocalIp()
{

	int fdI; 			// Socket file descriptor
	struct ifreq req; 	// Struct for system request
	char ip[12]; 		// Char for IP to set
	int i=1; 			// Counter variable 1
	int j=1; 			// Counter variable 2
	
	// Aufbauregister
	static char macIDtable[10][7] = {
	//  { ID | MAC ADDRESS				   } ID = Aufbaunummer = Last Number of IP
		{0x00,0x00,0x08,0x9b,0x00,0x00,0x00},	// n.a.
		{0x01,0x00,0x08,0x9b,0x6a,0x10,0x34}, 	// #1
		{0x02,0x00,0x08,0x9b,0x6a,0x0f,0x3c}, 	// #2
		{0x03,0x00,0x08,0x9b,0x6a,0x10,0x22}, 	// #3
		{0x04,0x00,0x08,0x9b,0x6a,0x0f,0x48}, 	// #4
		{0x05,0x00,0x08,0x9b,0x6a,0x0f,0xc0}, 	// #5
		{0x06,0x00,0x08,0x9b,0x6a,0x0e,0x78}, 	// #6
		{0x07,0x00,0x08,0x9b,0x6a,0x0f,0xee}, 	// #7
		{0x08,0x00,0x08,0x9b,0x6a,0x10,0x1c}, 	// #8
		{0x09,0x00,0x08,0x9b,0x6a,0x9f,0xca} 	// #9	
		};
	
	fdI = socket( AF_INET, SOCK_RAW, 0 );

	(void)bzero( (char*)&req, sizeof(req) );
	strncpy( req.ifr_name, "fei1", strlen("fei1") );
	ioctl( fdI, SIOCGIFLLADDR, &req );
	   
	printf("HWaddr %02x%02x%02x%02x%02x%02x\n", 
					  (unsigned char)req.ifr_addr.sa_data[0], (unsigned char)req.ifr_addr.sa_data[1], 
					  (unsigned char)req.ifr_addr.sa_data[2], (unsigned char)req.ifr_addr.sa_data[3], 
					  (unsigned char)req.ifr_addr.sa_data[4], (unsigned char)req.ifr_addr.sa_data[5]);
	

	// Loop through table
	while (i <= 10) 
		{
			while (j <= 6) 
				{
					if ( req.ifr_addr.sa_data[j-1] != macIDtable[i][j] ) j = 100; // Break inner while, if mac address differs
					
					j++;
				} // End while j
			
			if (j == 7)  // True if mac id is identical --> set correct ip
				{
					sprintf(ip,"91.0.0.%i",i);				
					printf("Setting IP: %s\n",ip);
					sprintf(myIPAddress,"%s",ip);
					ifAddrSet("fei1",ip);
					i = 100; // Break outer while
				}				
			j=1; 	// Reset inner counter var
			i++;	// Go to next line
		} // End while i
	
	if (j == 100) // True when no matching MAC address was found 
		{
			printf("MAC ID unknown, setting IP to 127.0.0.1.\n");
			ifAddrSet("fei1","127.0.0.1");
		}
	
	close(fdI);
	return;	
}
