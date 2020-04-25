#ifndef CLIENT_H
#define CLIENT_H

#include <list>
#include <memory>
#include <mutex>
#include <cstdint>
#include <future>
#include "Definitions.h"
#include "Buffer.h"

using namespace std;

class Client
{
public:
	void update();

	Client(Client& client);
	Client(const sockPtr& socket);
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
	void loginRequest(Buffer& buffer);
	void registerRequest(Buffer& buffer);
	void playerInput(Buffer& buffer);
	void playerAttRequest(Buffer& buffer);
	void msgHandler(Buffer& buffer, const int& size);
	
	friend class Server;
};

#endif