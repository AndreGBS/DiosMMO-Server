#include <list>
#include <memory>
#include "Logger.h"
#include "Client.h"
#include "Server.h"
#include "Buffer.h"
#include "Definitions.h"
#include <future>
#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <string>

using namespace std;

Client::Client(const sockPtr& socket)
	: socket(socket)
{
	LOG("Um cliente conectou");
}

Client::Client(Client& client)
{
	throw runtime_error("CLIENT NAO PODE SER COPIADO");
}

Client::~Client()
{
	LOG("Um cliente desconectou");
	//closesocket(*socket);
}

void Client::listen()
{
	int packetSize;
	char* buffer = new char[DEFAULT_BUFLEN];
	Buffer buff(buffer);
    do 
    { 
        packetSize = read(*socket, buffer, DEFAULT_BUFLEN);
        if (packetSize > 0)
        {
			buff.seek(0);
        	msgHandler(buff, packetSize);           
        }
        else if (packetSize == 0)
        {
            LOG("Um cliente desconectou");
            close(*socket);
            return;
        }
        else
        {
        	LOG("Falha na conexão");
           	close(*socket);
            return;
        }
    } while (packetSize > 0 && !SERVER_EXITED);
    delete [] buffer;
}

void Client::startListen()
{
	clientThread = async(&Client::listen, this);
}

void Client::msgHandler(Buffer& buffer, const int& size)
{
	while(buffer.tell() < size)
	{
		switch(buffer.read<uint8_t>())
		{
			case MESSID_LOGIN_REQUEST:
				loginRequest(buffer);
			break;

			case MESSID_REGISTER_REQUEST:
				registerRequest(buffer);
			break;

			case MESSID_PLAYER_INPUT:
				playerInput(buffer);
			break;

			case MESSID_PLAYER_ATT_REQUEST:
				playerAttRequest(buffer);
			break;
		}
	}
}

void Client::loginRequest(Buffer& buffer)
{
	string username = buffer.readString();
	string password = buffer.readString();
	LOG(username);
	LOG(password);
	Server* server = Server::getInstance();

	string queryStr = 
		"SELECT * FROM accounts WHERE username = '" + username + "' AND password = '" + password + "'";

	pqxx::row row = server->query1(queryStr);

	char response[2];
	response[0] = MESSID_LOGIN_REQUEST;
	response[1] = false;

	if(row.size() > 0)
	{
		PID = Server::getInstance()->getNewPID(_this);
		if(PID != INVALID_PID)
		{
			response[1] = true;
			LOG("Logou com sucesso com o PID" + to_string(PID));
		}
		else
		{
			LOG("Servidor cheio");
		}
	}
	else
	{
		LOG("Falha ao logar");
	}

  	int bytesSent = send(*socket, response, 2, 0);
    if (bytesSent < 0)
    {
        LOG("Falha ao enviar o pacote");
        close(*socket);
    } 
}

void Client::registerRequest(Buffer& buffer)
{
	string username = buffer.readString();
	string password = buffer.readString();

	Server* server = Server::getInstance();

	string queryStr = 
		"INSERT INTO accounts (username, password) VALUES ('" + username + "', '" + password + "')";

	char response[2];
	response[0] = MESSID_REGISTER_REQUEST;
	if(server->insertQuery(queryStr))
	{
		response[1] = true;
		LOG("Cadastro realizado com sucesso");
	}
	else
	{
		response[1] = false;
		LOG("Falha no cadastro");
	}

   	int bytesSent = send(*socket, response, 2, 0);
    if (bytesSent < 0)
    {
        LOG("Falha ao enviar o pacote");
        close(*socket);
    }  
} 

void Client::playerInput(Buffer& buffer)
{
	updateMutex.lock();
	for(int i = 0;i < 4; ++i)
		inputs[i] = buffer.read<uint8_t>();
	updateMutex.unlock();
}

void Client::playerAttRequest(Buffer& buffer)
{
	updateMutex.lock();
	int posX = (int)position[0];
	int posY = (int)position[1];
	updateMutex.unlock();

	char response[sizeof(int)*2+sizeof(uint16_t)+1];

	response[0] = MESSID_PLAYER_ATT_REQUEST;
	memcpy(response+1, &PID, sizeof(uint16_t));
	memcpy(response+1, &posX, sizeof(int));
	memcpy(response+1+sizeof(int), &posY, sizeof(int));

    int bytesSent = send(*socket, response, sizeof(response), 0);
    if (bytesSent < 0)
    {
        LOG("Falha ao enviar o pacote");
        close(*socket);
    } 
}

void Client::update()
{
	const float speed = 2;

	updateMutex.lock();
	int hDir = inputs[0] - inputs[2];
	int vDir = inputs[3] - inputs[1];
	position[0] += hDir * 0.2;
	position[1] += vDir * 0.2;
	updateMutex.unlock();
}