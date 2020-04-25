#include <thread>
#include <list>
#include <memory>
#include "Logger.h"
#include "Client.h"
#include "Definitions.h"
#include "UpdateManager.h"
#include "Server.h"
#include <future>
#include <iostream>
#include <mutex>
#include <unistd.h>

using namespace std;

UpdateManager::UpdateManager()
{
	LOG("UpdateManager iniciado");
}

UpdateManager::UpdateManager(UpdateManager& updateManager)
{
	throw runtime_error("UpdateManager não pode ser copiado");
}

UpdateManager::UpdateManager(UpdateManager&& updateManager)
{
}

UpdateManager::~UpdateManager()
{
}

void UpdateManager::startUpdate()
{
	updateThread = async(&UpdateManager::update, this);
}

void UpdateManager::update()
{
	const list<clientPtr>& clientList = Server::getInstance()->getClientList(); 
    while(!SERVER_EXITED)
    {
        for(const auto& client : clientList)
            client->update();
        usleep(15);
    } 
}
