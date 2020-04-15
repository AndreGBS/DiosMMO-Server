#ifndef DEFINITIONS
#define DEFINITIONS

#pragma comment(lib, "Ws2_32.lib")

#define LOG(msg) Logger::log(__PRETTY_FUNCTION__, msg);
#define SERVER_VERSION 0.1
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define SERVER_EXITED (Server::getInstance()->getExitSignal().wait_for(std::chrono::milliseconds(1)) != future_status::timeout)

#define MESSID_LOGIN_REQUEST 0
#define MESSID_REGISTER_REQUEST 1

#endif