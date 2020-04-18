#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <future>

using namespace std;

class UpdateManager
{
public:
	void update();
	~UpdateManager();

private:
	future<void> updateThread;

	void startUpdate();

	UpdateManager();
	UpdateManager(UpdateManager& updateManager);
	UpdateManager(UpdateManager&& updateManager);
	
	friend class Server;
};

#endif