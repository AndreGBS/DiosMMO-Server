#include <iostream>
#include "Server.h"
#include "Logger.h"
#include <string>
#include "Definitions.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
	Logger* logger = Logger::getInstance();
	Server* sv = Server::getInstance();

	if(sv->initialize())
	{
		LOG("Servidor iniciado com sucesso");
	}
	else
	{
		sv->close();
	}

	sv->wait();
	sv->close();
	logger->destroy();
}