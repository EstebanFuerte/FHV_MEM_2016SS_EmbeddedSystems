/* tcpClient.c - TCP client example 
	wr_net_stack_vxworks_6_programmers_guide_2.1 p.196-198 */
	
/*
DESCRIPTION
This file contains the client-side of the TCP example code.
The example code demonstrates the usage of several BSD 4.4-style
socket routine calls.
*/

/* includes */
#include "vxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "taskLib.h"
#include "stdioLib.h"
#include "strLib.h"
#include "hostLib.h"
#include "ioLib.h"
#include "fioLib.h"

#include "../sm/systemManager.h"
#include "../sm/stateMachine.h"
#include "TCP_Client.h"


//const char *serverName;
//sprintf(serverName,"91.0.0.105");


StateMachine * myStateMachine;
STATUS tcpClient(char * serverName);
STATUS send_Request(int sFdClient, char * message);
int sFdClient=0;
char ipAdress[20];




//c++ Methods
TCP_Client :: TCP_Client(){				// Konstruktor
	return;
}

TCP_Client :: ~TCP_Client(){			// Dekonstruktor
	return;
}

void TCP_Client :: init(){
	strcpy(ipAdress,"91.0.0.105");
	taskSpawn("tcpClient",140,0,0x1000, (FUNCPTR) tcpClient,(int)ipAdress,0,0,0,0,0,0,0,0,0);
	printf("TCP_Client :: init() serverName: %s\n\r",ipAdress);
	return;
}

void TCP_Client :: sendMessage(char * message){
	send_Request(sFdClient,message);
	return;
}
/****************************************************************************
*
* tcpClient - send requests to server over a TCP socket
*
* This routine connects over a TCP socket to a server, and sends a
* user-provided message to the server. Optionally, this routine
* waits for the server's reply message.
*
* This routine may be invoked as follows:
* -> tcpClient "remoteSystem"
* Message to send:
* Hello out there
* Would you like a reply (Y or N):
* y
* value = 0 = 0x0
* -> MESSAGE FROM SERVER:
* Server received your message
*
* RETURNS: OK, or ERROR if the message could not be sent to the server.
*/
STATUS tcpClient
	(
	char * serverName /* name or IP address of server */
	)
	{
	int nRead;
	printf("TCP_Client: STATUS tcpClient, serverName: %s\r\n",serverName);
	
	//struct request myRequest; /* request to send to server */
	struct sockaddr_in serverAddr; /* server's socket address */
	char replyBuf[REPLY_MSG_SIZE]; /* buffer for reply */
	char reply; /* if TRUE, expect reply back */
	int sockAddrSize; /* size of socket address structure */
	//int sFdClient; /* socket file descriptor */
	int mlen; /* length of message */
	
	/* create client's socket */
	if ((sFdClient = socket (AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror ("socket");
		return (ERROR);
	}
	
	//printout Client socket
	printf("TCP_Client: tcpClient - Socket: %d\r\n", sFdClient);
	
	/* bind not required - port number is dynamic */
	/* build server socket address */
	sockAddrSize = sizeof (struct sockaddr_in);
	bzero ((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_len = (u_char) sockAddrSize;
	serverAddr.sin_port = htons (SERVER_PORT_NUM_TCP);
	
	if (((serverAddr.sin_addr.s_addr = inet_addr (serverName)) == ERROR) &&
	((serverAddr.sin_addr.s_addr = hostGetByName (serverName)) == ERROR))
	{
		perror ("TCP_Client: unknown server name:");
		printf ("TCP_Client: serverName:\n%s\n", serverName);
		close (sFdClient);
		return (ERROR);
	}
	
	/* connect to server */
	if (connect (sFdClient, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR)
	{
		perror ("connect");
		close (sFdClient);
		return (ERROR);
	}
	printf("TCP_Client: connectedToServer:%s\n\r",serverName);
	
	/* build request, prompting user for message 
	printf ("Message to send: \n");
	mlen = read (STD_IN, myRequest.message, REQUEST_MSG_SIZE);
	myRequest.msgLen = mlen;
	myRequest.message[mlen - 1] = '\0';
	printf ("Would you like a reply (Y or N): \n");
	read (STD_IN, &reply, 1);
	switch (reply)
	{
		case 'y':
		case 'Y': myRequest.reply = TRUE;
		break;
		default: myRequest.reply = FALSE;
		break;
	}*/
	
	/* send request to server */
	/*if (write (sFd, (char *) &myRequest, sizeof (myRequest)) == ERROR)
	{
		perror ("write");
		close (sFd);
		return (ERROR);
	}*/
	/*if (myRequest.reply) // if expecting reply, read and display it 
	{*/
	while ((nRead = fioRdString(sFdClient,  (char *) &replyBuf, sizeof(replyBuf))) > 0) {
	//if (read (sFdClient, replyBuf, REPLY_MSG_SIZE) < 0)
	/*{
		perror ("read");
		//close (sFdClient);
		return (ERROR);
	}
	else {*/
		printf("TCP_Client: im read -> rplyBuf !=0: %s\n\r",replyBuf);
		
		if(strcmp(replyBuf,"RELEASE\r")==0)
		{
			printf("TCP-Client; receiveRelease; %d\r\n",sFdClient);
			myStateMachine->sendEvent("receiveRelease");
		}
		else if(strcmp(replyBuf,"WAIT\r")==0){
			printf("TCP-Client: Wait; %d\r\n",sFdClient);
			myStateMachine->sendEvent("receiveWait");
		}
		else if (strcmp(replyBuf,"READY\r")==0){
			printf("TCP-Client: Ready; %d\r\n",sFdClient);
			myStateMachine->sendEvent("receiveReady");
		}
	//}
	//printf ("MESSAGE FROM SERVER:\n%s\n", replyBuf);
	}
	//close (sFd);
	return (OK);
}

STATUS send_Request(int sFdClient, char * message){
	
	char sendBuffer[256];
	sprintf(sendBuffer,message);
	
	printf("TCP_Client: send_Request: %s, %d\n\r",sendBuffer, sFdClient);
	
	/*
	 *	Send from TCP-Server
	 * 	static char errorMsg[] = "TCP-Server: Falsche Eingabe; bitte Eingaben nach Tabelle xy betätigen\n\r";
		write(sFdServer, errorMsg, strlen(errorMsg));
	 */
	
	if (write (sFdClient, sendBuffer, strlen(sendBuffer)) == ERROR)
	{
		perror ("write");
		close (sFdClient);
		return (ERROR);
	}
	else
	{
		//write(sFdClient, sendBuffer, strlen(sendBuffer));
		printf("TCP_Client: send_Request done! sendBuffer: %s %d\n\r",sendBuffer,sFdClient);
		//close (sFd);
		return (OK);
	}
	
}
