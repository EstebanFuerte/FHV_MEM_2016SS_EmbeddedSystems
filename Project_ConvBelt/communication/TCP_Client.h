#define SERVER_PORT_NUM_TCP		5555	/* server's port number for bind() */
#define REPLY_MSG_SIZE 			500 	/* max size of reply message */	
	
class TCP_client{
public:	
	TCP_client();
	~TCP_Client();
	void sendMessage(char *message);
}