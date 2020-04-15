#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <future>
#include <list>
#include <memory>
#include "Client.h"
#include <pqxx/pqxx>

using namespace std;

typedef shared_ptr<Client> clientPtr;

class Server
{
public:
	bool initialize();
	void wait();

	static Server* getInstance();
	static void close();

	const future<void>& getExitSignal();

	bool insertQuery(const string& queryStr);
	const pqxx::row query1(const string& queryStr);
	const pqxx::result query(const string& queryStr);	

private:
	pqxx::connection* pqConn;
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