#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <list>
#include <memory>

using namespace std;

typedef shared_ptr<SOCKET> sockPtr;

class Client
{
public:
	Client(Client& client);
	Client(sockPtr socket);
	~Client();

private:
	sockPtr socket;
	thread clientThread;
	float position[2] = {200.0f, 200.0f};
	bool inputs[4] = {false};
	void listen();
	void startListen();
	void loginRequest(const char * buffer);
	void registerRequest(const char* buffer);
	void playerInput(const char* buffer);
	void playerAttRequest(const char* buffer);
	void msgHandler(const char * buffer);

	void update();
	
	friend class Server;
};

#endif