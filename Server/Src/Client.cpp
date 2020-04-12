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
        	Logger::print("Pacote de tamanho " + to_string(iResult) + " foi recebido :" + buffer);
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
	
	Logger::print("Username: " + username);
	Logger::print("Password: " + password);

	const int responseSize = 2;
	char response[responseSize];
	response[0] = MESSID_LOGIN_REQUEST;
	response[1] = (username.compare("Hitsuji") == 0 && password.compare("123") == 0) ? true : false;

    int iResult = send(*socket, response, responseSize, 0);

    if (iResult == SOCKET_ERROR)
    {
        LOG("Falha ao enviar o pacote");
        closesocket(*socket);
    }        
}