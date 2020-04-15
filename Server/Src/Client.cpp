#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <list>
#include <memory>
#include "Logger.h"
#include "Client.h"
#include "Server.h"
#include "windows.h"
#include "Definitions.h"
#include <future>
#include <iostream>

using namespace std;

Client::Client(sockPtr socket)
	: socket(socket)
{
	LOG("Um cliente conectou");
}

Client::Client(Client& client)
{
	socket = client.socket;
	clientThread.swap(client.clientThread);
}

Client::~Client()
{
	LOG("Um cliente desconectou");
	closesocket(*socket);
}

void Client::listen()
{
	int iResult;
	char* buffer = new char[DEFAULT_BUFLEN];
    do 
    { 
        int iSendResult;
        int length = DEFAULT_BUFLEN;

        iResult = recv(*socket, buffer, length, 0);
        if (iResult > 0)
        {
           	msgHandler(buffer);           
        }
        else if (iResult == 0)
        {
            LOG("Um cliente desconectou");
            closesocket(*socket);
            return;
        }
        else
        {
        	LOG("Falha na conexão");
            closesocket(*socket);
            return;
        }
    } while (iResult > 0 && !SERVER_EXITED);
    delete [] buffer;
}

void Client::startListen()
{
	clientThread = thread(listen, this);
	clientThread.join();
}

void Client::msgHandler(const char* buffer)
{
	int messid = buffer[0];
	switch(messid)
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

void Client::loginRequest(const char* buffer)
{
	string username;
	string password;

	int index = 1;
	while(buffer[index] != 0)
	{
		username += buffer[index];
		++index;
	}
	++index;
	while(buffer[index] != 0)
	{
		password += buffer[index];
		++index;
	}
	
	Server* server = Server::getInstance();

	string queryStr = 
		"SELECT * FROM accounts WHERE username = '" + username + "' AND password = '" + password + "'";

	pqxx::row row = server->query1(queryStr);

	char response[2];
	response[0] = MESSID_LOGIN_REQUEST;
	if(row.size() > 0)
	{
		response[1] = true;
	}
	else
	{
		response[1] = false;
	}

    int iResult = send(*socket, response, 2, 0);
    if (iResult == SOCKET_ERROR)
    {
        LOG("Falha ao enviar o pacote");
        closesocket(*socket);
    }        
}

void Client::registerRequest(const char* buffer)
{
	string username;
	string password;

	int index = 1;
	while(buffer[index] != 0)
	{
		username += buffer[index];
		++index;
	}
	++index;
	while(buffer[index] != 0)
	{
		password += buffer[index];
		++index;
	}
	
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

    int iResult = send(*socket, response, 2, 0);
    if (iResult == SOCKET_ERROR)
    {
        LOG("Falha ao enviar o pacote");
        closesocket(*socket);
    }        
}

void Client::playerInput(const char* buffer)
{
	for(int i = 0;i < 4; ++i)
		inputs[i] = buffer[i+1];
}

void Client::playerAttRequest(const char* buffer)
{
	int posX = (int)position[0];
	int posY = (int)position[1];
	char response[sizeof(int)*2+1];

	response[0] = MESSID_PLAYER_ATT_REQUEST;
	memcpy(response+1, &posX, sizeof(int));
	memcpy(response+1+sizeof(int), &posY, sizeof(int));

    int iResult = send(*socket, response, sizeof(response), 0);
    if (iResult == SOCKET_ERROR)
    {
        LOG("Falha ao enviar o pacote");
        closesocket(*socket);
    } 
}

void Client::update()
{
	const float speed = 2;
	int hDir = inputs[0] - inputs[2];
	int vDir = inputs[3] - inputs[1];
	position[0] += hDir * 2;
	position[1] += vDir * 2;
}