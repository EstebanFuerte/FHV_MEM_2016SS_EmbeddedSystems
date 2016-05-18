#define SERVER_PORT_NUM_TCP		5555 /* server's port number for bind() */
#define SERVER_WORK_PRIORITY 	100 /* priority of server's work task */
#define SERVER_STACK_SIZE 		10000 /* stack size of server's work task */
#define SERVER_MAX_CONNECTIONS 	4 /* max clients connected at a time */
#define REQUEST_MSG_SIZE 		1024 /* max size of request message */
#define REPLY_MSG_SIZE			500
	
class TCP_Client{
public:	
	TCP_Client();
	~TCP_Client();
	void init();
	void sendMessage(char *message);
};
