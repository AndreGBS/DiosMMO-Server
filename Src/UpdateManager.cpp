#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <list>
#include <memory>
#include "Logger.h"
#include "Client.h"
#include "Server.h"
#include "UpdateManager.h"
#include "windows.h"
#include "Logger.h"
#include "Definitions.h"
#include <future>
#include <iostream>
#include <mutex>

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
	updateThread = async(update, this);
}

void UpdateManager::update()
{
	const list<clientPtr>& clientList = Server::getInstance()->getClientList(); 
    while(!SERVER_EXITED)
    {
        for(const auto& client : clientList)
            client->update();
        Sleep(16);
    } 
}
