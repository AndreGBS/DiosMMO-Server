#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <list>
#include <memory>
#include <mutex>
#include <cstdint>
#include <future>

using namespace std;

typedef shared_ptr<SOCKET> sockPtr;

class Client
{
public:
	void update();

	Client(Client& client);
	Client(sockPtr socket);
	~Client();

private:
	shared_ptr<Client> _this;
	uint16_t PID = 0;
	sockPtr socket;
	future<void> clientThread;
	mutex updateMutex;
	float position[2] = {200.0f, 200.0f};
	bool inputs[4] = {false};
	void listen();
	void startListen();
	void loginRequest(const char * buffer);
	void registerRequest(const char* buffer);
	void playerInput(const char* buffer);
	void playerAttRequest(const char* buffer);
	void msgHandler(const char * buffer);
	
	friend class Server;
};

#endif