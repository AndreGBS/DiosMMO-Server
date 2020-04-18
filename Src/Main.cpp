#include <iostream>
#include "Server.h"
#include "Logger.h"
#include <string>
#include "Definitions.h"
#include "Buffer.h"

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
		return false;
	}

	sv->wait();
	sv->close();
	logger->destroy();
}