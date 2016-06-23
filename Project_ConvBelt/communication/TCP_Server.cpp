/* Author: Stefan Stark
 * Source: Wind River Network Stack for VxWorks 6 - page 198 
 */

/* includes */
#include "vxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "taskLib.h"
#include "stdioLib.h"
#include "strLib.h"
#include "ioLib.h"
#include "fioLib.h"
//#include "tcpExample.h"

#include "../sm/systemManager.h"
#include "../sm/stateMachine.h"
#include "TCP_Server.h"
#include "TCP_Client.h"

StateMachine * myStateMachine;
TCP_Client * myTCPClient;

STATUS tcpServer(void);
void tcpServerSendReply(int sFdServer, char * message);
int sFdS;

TCP_Server :: TCP_Server() {	//Konstruktor zum Speicher reservieren
	return;
}

TCP_Server :: ~TCP_Server() {
	return;
}

void TCP_Server :: init(){		//Init zum starten des tasks und aufrufen des status
	taskSpawn("tcpServer",104,0,0x1000, (FUNCPTR) tcpServer,0,0,0,0,0,0,0,0,0,0);
	return;
}

void TCP_Server :: sendMessage(char *message){
	tcpServerSendReply(sFdS, message);
	return;
}

/* function declarations */
VOID tcpServerWorkTask(int sFdServer, char * address, u_short port);

/****************************************************************************
 *
 * tcpServer - accept and process requests over a TCP socket
 *
 * This routine creates a TCP socket, and accepts connections over the socket
 * from clients. Each client connection is handled by spawning a separate
 * task to handle client requests.
 *
 * This routine may be invoked as follows:
 * -> sp tcpServer
 * task spawned: id = 0x3a6f1c, name = t1
 * value = 3829532 = 0x3a6f1c
 * -> MESSAGE FROM CLIENT (Internet Address 192.0.2.10, port 1027):
 * Hello out there
 *
 * RETURNS: Never, or ERROR if a resources could not be allocated.
 */

STATUS tcpServer(void) {
	
	
	
	struct sockaddr_in 	serverAddr; /* server's socket address */
	struct sockaddr_in 	clientAddr; /* client's socket address */
	int 				sockAddrSize; /* size of socket address structure */
	int 				sFdServer; /* socket file descriptor */
	int 				newFd; /* socket descriptor from accept */
	int 				ix = 0; /* counter for work task names */
	char 				workName[16]; /* name of work task */

	/* set up the local address */
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_len = (u_char) sockAddrSize;
	serverAddr.sin_port = htons(SERVER_PORT_NUM_TCP);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* create a TCP-based socket */
	if ((sFdServer = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		perror("socket");
		return (ERROR);
	}
	
	//printout Server socket
	printf("TCP_Server: tcpServer - Socket: %d\r\n", sFdServer);

	/* bind socket to local address */
	if (bind(sFdServer, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR) {
		perror("bind");
		close(sFdServer);
		return (ERROR);
	}

	/* create queue for client connection requests */
	if (listen(sFdServer, SERVER_MAX_CONNECTIONS) == ERROR) {
		perror("listen");
		close(sFdServer);
		return (ERROR);
	}

	/* accept new connect requests and spawn tasks to process them */
	while (TRUE) {	//Anstatt FOREVER
			socklen_t len = sizeof(clientAddr); //Muss neu hinzugefügt werden, damit mehrer Verbindungen aufgebaut werden können
		if ((newFd = accept(sFdServer, (struct sockaddr *) &clientAddr, &len))
				== ERROR) {
			perror("accept");
			close(sFdServer);
			return (ERROR);
		}
		sprintf(workName, "tTcpWork%d", ix++);
		if (taskSpawn(workName, SERVER_WORK_PRIORITY, 0, SERVER_STACK_SIZE,
				(FUNCPTR) tcpServerWorkTask, newFd,
				(int) inet_ntoa(clientAddr.sin_addr),
				ntohs(clientAddr.sin_port), 0, 0, 0, 0, 0, 0, 0) == ERROR) {
			/* if taskSpawn fails, close fd and return to top of loop */
			perror("taskSpawn");
			close(newFd);
		}
	}
	
	//printf("tcpServer initialized\r\n");
}

/****************************************************************************
 *
 * tcpServerWorkTask - process client requests
 *
 * This routine reads from the server's socket, and processes client
 * requests. If the client requests a reply message, this routine
 * will send a reply to the client.
 *
 * RETURNS: N/A.
 */

VOID tcpServerWorkTask
	(
	int 			sFd, 		/* server's socket fd */
	char * 			address, 	/* client's socket address */
	u_short 		port 		/* client's socket port */
	) 
	{
	//struct request 	clientRequest; /* request/message from client */
	int 			nRead; /* number of bytes read */
	//static char 	replyMsg[] = "Server received your message";
	
	
	printf("tcpServerWorkTask- sFdServer:%i\n\r",sFd);
	
	char clientRequest[256];
	char replyMsg[256];
	static char welcomeMsg[] = "Welcome to the TCP Server.\n\r";
	write(sFd, welcomeMsg, sizeof(welcomeMsg));

	// Set connection to client, show Client ip
	char addresClient[20];
	//char addresClient2[20];
	
	//sprintf(addresClient2,address);
	//sprintf(addresClient,"Client-IP: %s\n\r",address);
	sprintf(addresClient,address);
	//write(sFd,addresClient,strlen(addresClient));
	
	if (strcmp(addresClient, "91.0.0.91") != 0) {
		sFdS = sFd;
		printf("overwrite sFdS with: %i ; iP-Adress: %s; addresClient: %s\n\r", sFdS, address, addresClient);
	}
	
	/* read client request, display message */
	//fioReadString = Funktion wo read aufruft wenn Enter gedrückt wird
	// while >0, 0 bedeutet Client hat sich abegemeldet         
	while ((nRead = fioRdString(sFd,  (char *) &clientRequest, sizeof(clientRequest))) > 0) {
		//printf("in while of TCP\r\n");
		
		printf("TCP_Server-ReceivedMsg: %s\n\r",clientRequest);
		
		if (strncmp(clientRequest, "Right",5)==0){				//parse IP
			char TCP_ClientIpAdress[20];
			sprintf(TCP_ClientIpAdress, strpbrk(clientRequest,".0123456789"));
			//printf("new TCP_CLIENTIPADRESS: %s\n\r", TCP_ClientIpAdress);
			myTCPClient->init(TCP_ClientIpAdress);
			
			
		}
		else if (strcmp(clientRequest,"Request\r")==0) {//strcmp to compare strings in c
			myStateMachine->sendEvent("receiveRequest");
			//printf("TCP-Server: request\n\r");
		}
		/*else if (strcmp(clientRequest,"wait\r")==0){			//for debugging only
			myStateMachine->sendEvent("receiveWait");
			printf("TCP-Server: wait\n\r");

		}
		else if (strcmp(clientRequest,"ready\r")==0){			//for debugging only
			myStateMachine->sendEvent("receiveReady");
			printf("TCP-Server: ready\n\r");

		}
		else if (strcmp(clientRequest,"release\r")==0){			//for debugging only
			myStateMachine->sendEvent("receiveRelease");
			printf("TCP-Server: release\n\r");
		}*/
		else{
			static char errorMsg[] = "TCP-Server: Falsche Eingabe; bitte Eingaben nach Tabelle xy betätigen\n\r";
			write(sFd, errorMsg, strlen(errorMsg));
		}
	}
	if (nRead == ERROR) /* error from read() */
		perror ("read");
			close( sFd); /* close server socket connection */
	}

void tcpServerSendReply(int sFdS, char * message){
	char sendBuffer[256];
	sprintf(sendBuffer,message);
	
	printf("tcpServerSendReply - sFdServer:%i, Server-Msg: %s", sFdS,sendBuffer);
	
	
	//strcmp to compare strings in c
	if (strcmp(sendBuffer,"Release\r\n")==0) {
		//printf("tcpServerSendReply: Release");
		static char msg[]= "Release\r\n";
		write(sFdS, msg, sizeof(msg));
	}
	else if (strcmp(sendBuffer,"Wait\r\n")==0){
		//printf("tcpServerSendReply: Wait");
		static char msg[]= "Wait\r\n";
		write(sFdS, msg, sizeof(msg));

	}
	else if (strcmp(sendBuffer,"Ready\r\n")==0){
		//printf("tcpServerSendReply: Ready");
		static char msg[]= "Ready\r\n";
		write(sFdS, msg, sizeof(msg));

	}
	else{
		static char errorMsg[] = " Unbekannte Msg\n\r";
		write(sFdS, errorMsg, strlen(errorMsg));
	}
}
