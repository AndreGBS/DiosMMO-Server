#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <future>
#include <list>
#include <memory>
#include "Client.h"

using namespace std;

typedef shared_ptr<Client> clientPtr;

class Server
{
public:
	static Server* getInstance();
	static void close();
	bool initialize();
	const future<void>& getExitSignal();
	void wait();

private:
	SOCKET listenSocket = INVALID_SOCKET;
	list<clientPtr> clientList;
	static Server* _this;
	bool waitingForConnections = false;	
    WSADATA wsaData;

	future<void> listenThread;
	future<void> consoleThread;

	promise<void> exitSignal;
	future<void> exitFuture;

	Server();
	Server(const Server& sv);
	Server(Server&& sv);
	~Server();

	void console();
	void waitForConnections();
};

#endif