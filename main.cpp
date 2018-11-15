#include <stdio.h>
#include <stdint.h>

#include "Socket.h"
#include "Debug.h"

#define HOST "127.0.0.1"
#define PORT 8888

void socketTest()
{
	Socket *sock;
	int clientfd;
	const size_t timeout_sec = 10;
	string toSend;
	unsigned char toReceive[128];

	Debug::print(Debug::CL_NORMAL, "Starting socket tests...\n");

	sock = new Socket(HOST, (uint16_t) PORT);
	toSend = "Hello, I'm the socket server!";

	if(sock->isOpened())
	{
		Debug::print(Debug::CL_NORMAL, "Start listening...\n");
		clientfd = sock->listenConnections(timeout_sec);
		if(clientfd > 0)
		{
			Debug::print(Debug::CL_NORMAL, "New client connection...\n");

			// Sending data...
			if(sock->sendData(clientfd, (const unsigned char *) toSend.c_str(), toSend.length()))
			{
				Debug::print(Debug::CL_NORMAL, "Data sent\n");
			}

			// Sending new line...
			if(sock->sendData(clientfd, (const unsigned char *) "\n", 1))
			{
				Debug::print(Debug::CL_NORMAL, "Newline sent\n");
			}

			// Receiving response...
			if(sock->receiveData(clientfd, toReceive, sizeof(toReceive), timeout_sec, true))
			{
				Debug::print(Debug::CL_NORMAL, "Socket client response: [%s]\n", toReceive);
			}

			Debug::print(Debug::CL_NORMAL, "Closing client connection...\n");
			sock->closeClientConnection(clientfd);
		}
	}

	Debug::print(Debug::CL_NORMAL, "Done!\n");

	delete sock;
}

int main(int argc, char *argv[])
{
	socketTest();
	return 0;
}
