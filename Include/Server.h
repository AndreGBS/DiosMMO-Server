#ifndef SERVER_H
#define SERVER_H

#include <future>
#include <list>
#include <memory>
#include "Client.h"
#include "UpdateManager.h"
#include <pqxx/pqxx>
#include <mutex>
#include <vector>
#include <cstdint>
#include "Definitions.h"
#include <netinet/in.h>

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
	const list<clientPtr>& getClientList();

	uint16_t getNewPID(const clientPtr client);
	
private:
	pqxx::connection* pqConn;
	int listenSocket;
	sockaddr_in  address;
	list<clientPtr> clientList;
	static Server* _this;
	bool waitingForConnections = false;	

    UpdateManager updateManager;

	future<void> listenThread;
	future<void> consoleThread;

	promise<void> exitSignal;
	future<void> exitFuture;

	clientPtr PIDs[MAX_PLAYERS]; //Player ID

	Server();
	Server(const Server& sv);
	Server(Server&& sv);
	~Server();

	void console();
	void waitForConnections();
};

#endif