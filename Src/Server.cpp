#include "stdio.h"
#include <iostream>
#include <future>
#include <chrono>
#include "Definitions.h"
#include "Server.h"
#include "Logger.h"
#include "Client.h"
#include <pqxx/pqxx>
#include <fstream>
#include <cstdint>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <memory>

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

    int opt = 1;
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(socket == 0)
    {
        LOG("Falha ao criar o listenSocket");
        return false;
    }

    if(setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        LOG("Falha ao seta opções do socket");
        return false;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEFAULT_PORT);

    if(bind(listenSocket, (sockaddr*) &address, sizeof(address)) < 0)
    {
        LOG("Falha no bind do listenSocket");
        return false;
    }

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

    exitFuture = exitSignal.get_future();
    updateManager.startUpdate();
    listenThread = async(&Server::waitForConnections, this);
    consoleThread = async(&Server::console, this);

    return true;
}

void Server::waitForConnections()
{
    LOG("Esperando por conexoes");

    socklen_t addrlen = sizeof(address);
    listen(listenSocket, 3);
    while(!SERVER_EXITED)
    {
        sockPtr clientSocket = make_shared<int>();

        *clientSocket = accept(listenSocket, (sockaddr*) &address, &addrlen);
        LOG("Um novo client esta tentando se conectar");
        if (*clientSocket < 0)
        {
            LOG("Erro ao aceitar nova conexao");
        }
        else
        {
            clientPtr client = make_shared<Client>(clientSocket);
            clientList.push_back(client);
            client->startListen();
            client->_this = client;
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

const list<clientPtr>& Server::getClientList()
{
    return clientList;
}

void Server::wait()
{
    exitFuture.wait();
}

uint16_t Server::getNewPID(const clientPtr client)
{
    for(int i = 0; i < MAX_PLAYERS; ++i)
    {
        if(PIDs[i] == nullptr)
        {
            PIDs[i] = client;
            return i;
        }
    }
    return INVALID_PID;
}