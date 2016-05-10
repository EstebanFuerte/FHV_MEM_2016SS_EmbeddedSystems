/* Author: Stefan Stark
 * Source: Wind River Network Stack for VxWorks 6 - page 198 - modified for TELNET
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
#include "tcpExample.h"

#incldue "../sm/systemManager.h"
#incldue "../sm/stateMachine.h"
#include "Telnet_Server.h"

Telnet_Server :: Telnet_Server() {	//Konstruktor zum Speicher reservieren
	return;
}

Telnet_Server :: ~Telnet_Server() {
	return;
}

void Telnet_Server :: init(){		//Init zum starten des tasks und aufrufen des status
	serverTask = taskSpawn("tTelnet",104,0,0x1000, (FUNCPTR) tServer,0,0,0,0,0,0,0,0,0,0);
	tcpServer();
}

/* function declarations */
VOID telnetServerWorkTask(int sFd, char * address, u_short port);


STATUS telnetServer(void) {
	
	printf("in telnetServer\n");
	
	struct sockaddr_in 	serverAddr; /* server's socket address */
	struct sockaddr_in 	clientAddr; /* client's socket address */
	int 				sockAddrSize; /* size of socket address structure */
	int 				sFd; /* socket file descriptor */
	int 				newFd; /* socket descriptor from accept */
	int 				ix = 0; /* counter for work task names */
	char 				workName[16]; /* name of work task */

	/* set up the local address */
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_len = (u_char) sockAddrSize;
	serverAddr.sin_port = htons(SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* create a TCP-based socket */
	if ((sFd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		perror("socket");
		return (ERROR);
	}

	/* bind socket to local address */
	if (bind(sFd, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR) {
		perror("bind");
		close(sFd);
		return (ERROR);
	}

	/* create queue for client connection requests */
	if (listen(sFd, SERVER_MAX_CONNECTIONS) == ERROR) {
		perror("listen");
		close(sFd);
		return (ERROR);
	}

	/* accept new connect requests and spawn tasks to process them */
	while (TRUE) {	//Anstatt FOREVER
			socklen_t len = sizeof(clientAddr); //Muss neu hinzugefügt werden, damit mehrer Verbindungen aufgebaut werden können
		if ((newFd = accept(sFd, (struct sockaddr *) &clientAddr, &len))
				== ERROR) {
			perror("accept");
			close(sFd);
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
}

VOID telnetServerWorkTask
	(
	int 			sFd, 		/* server's socket fd */
	char * 			address, 	/* client's socket address */
	u_short 		port 		/* client's socket port */
	) 
	{
	//struct request 	clientRequest; /* request/message from client */
	int 			nRead; /* number of bytes read */
	//static char 	replyMsg[] = "Server received your message";
	
	char clientRequest[256];
	char replyMsg[256];
	static char welcomeMsg[] = "Welcome to the Server. Enter your request:\n\r";
	
	write(sFd, welcomeMsg, sizeof(welcomeMsg));
	
	/* read client request, display message */
	//fioReadString = Funktion wo read aufruft wenn Enter gedrückt wird
	// while >0, 0 bedeutet Client hat sich abegemeldet         
	while ((nRead = fioRdString(sFd,  (char *) &clientRequest, sizeof(clientRequest))) > 0) {
		
		if (clientRequest[0] == 'request') {
			printf("Key5IsPressed!\r\n");
			myStateMachine->sendEvent("receiveRequest");
		}
		else if (clientRequest[0] == 'wait'){
			myStateMachine->sendEvent("receiveWait");

		}
		else if (clientRequest[0] == 'ready'){
			myStateMachine->sendEvent("receiveReady");

		}
		else if (clientRequest[0] == 'release'){
			myStateMachine->sendEvent("receiveRelease");
		}
		else{
			static char errorMsg[] = " Falsche Eingabe; bitte Eingaben nach Tabelle xy betätigen\n\r";
			write(sFd, errorMsg, strlen(errorMsg));
		}
	}
	if (nRead == ERROR) /* error from read() */
		perror ("read");
			close( sFd); /* close server socket connection */
	}
