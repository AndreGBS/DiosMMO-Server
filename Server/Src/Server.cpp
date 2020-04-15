#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <future>
#include <chrono>
#include "Definitions.h"
#include "Server.h"
#include "Logger.h"
#include "Client.h"
#include <pqxx/pqxx>
#include <fstream>

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
    delete pqConn;
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

    LOG("Conectando ao banco de dados");
    try
    {
        ifstream options("options.txt");
        char credentials[256];
        options.getline(credentials, 256);
        pqConn = new pqxx::connection(credentials);
    }
    catch(const exception& e)
    {
        LOG("Falha ao conectar com o banco de dados");
        LOG(e.what());
        return false;
    }
    LOG("Conectou com o banco de dados");
    
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
    updateThread = async(update, this);

    return true;
}

void Server::waitForConnections()
{
    LOG("Esperando por conexoes");

    while(!SERVER_EXITED)
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
    while(!SERVER_EXITED)
    {
        string command;
        cin >> command;
        if(command.compare("quit") == 0)
            exitSignal.set_value();
    }
}

void Server::update()
{
    while(!SERVER_EXITED)
    {
        for(const auto& client : clientList)
            client->update();
        Sleep(16);
    }   
}

const future<void>& Server::getExitSignal()
{
    return exitFuture;
}

bool Server::insertQuery(const string& queryStr)
{
    try
    {
        pqxx::work query(*pqConn);
        query.exec0(queryStr); 
        query.commit();
        return true;
    }
    catch(const exception& e)
    {
        LOG("Exception " + string(e.what()));
        return false;
    }
}

const pqxx::row Server::query1(const string& queryStr)
{
    pqxx::row result;
    try
    {
        pqxx::work query(*pqConn);
        result = query.exec1(queryStr); 
    }
    catch(const exception& e)
    {
        LOG("Exception " + string(e.what()));
    }
    return result;
}

void Server::wait()
{
    exitFuture.wait();
}
