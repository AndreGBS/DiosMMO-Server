#include "Server.h"
#include "Logger.h"
#include "Client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <future>
#include "Definitions.h"
#include <chrono>

using namespace std;

Server* Server::_this = nullptr;

Server::Server()
{
    LOG("Instancia do servidor criada");
}

Server::Server(const Server& sv)
{ 
}

Server::Server(Server&& sv)
{
}

Server::~Server()
{
    Logger::print("Finalizando o server");
    WSACancelBlockingCall();
    shutdown(listenSocket, 1);
    closesocket(listenSocket);
    WSACleanup(); 
    Logger::print("Server finalizado");
}

void Server::close()
{
    if(_this != nullptr)
    {
        delete _this;
    }
    else
    {
        Logger::print("Nenhum server para finalizar");
    }
}

Server* Server::getInstance()
{
    if(_this == nullptr)
        _this = new Server();

    return _this;
}

bool Server::initialize()
{
    LOG("Iniciando o servidor");

    int iResult;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        LOG("Erro " + to_string(iResult) + " na inicialização do servidor");
        return false;
    }

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        LOG("Erro " + to_string(iResult) + " na inicialização do servidor");
        return false;
    }

    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET)
    {
        LOG("Erro " + to_string(WSAGetLastError()) + " na inicialização do socket");
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    iResult = bind( _this->listenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (listen(listenSocket, SOMAXCONN ) == SOCKET_ERROR)
    {
        LOG("Falha na inicalização do socket");
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }

    exitFuture = exitSignal.get_future();
    listenThread = async(waitForConnections, this);
    consoleThread = async(console, this);

    return true;
}

void Server::waitForConnections()
{
    LOG("Esperando por conexoes");

    while(true && !SERVER_EXITED)
    {
        sockPtr clientSocket = make_shared<SOCKET>();

        *clientSocket = accept(listenSocket, NULL, NULL);
        if (*clientSocket == INVALID_SOCKET)
        {
            LOG("Erro" + to_string(WSAGetLastError()) +" ao aceitar nova conexao");
        }
        else
        {
            clientPtr client = make_shared<Client>(clientSocket);
            clientList.push_back(client);
            client->startListen();
        }
    }
}

void Server::console()
{
    LOG("Console iniciado");
    while(true && !SERVER_EXITED)
    {
        string command;
        cin >> command;
        if(command.compare("quit") == 0)
            exitSignal.set_value();
    }
}

const future<void>& Server::getExitSignal()
{
    return exitFuture;
}

void Server::wait()
{
    exitFuture.wait();
}


