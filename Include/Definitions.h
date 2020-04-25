#ifndef DEFINITIONS
#define DEFINITIONS

#include <memory>

using namespace std;
typedef shared_ptr<int> sockPtr;

#define LOG(msg) cout << msg << endl;
//#define LOG(msg) Logger::log(__PRETTY_FUNCTION__, msg);

#define SERVER_VERSION 0.1
#define DEFAULT_PORT 27015
#define DEFAULT_BUFLEN 512
#define SERVER_EXITED (Server::getInstance()->getExitSignal().wait_for(std::chrono::milliseconds(1)) != future_status::timeout)
#define MAX_PLAYERS 100

#define MESSID_LOGIN_REQUEST 0
#define MESSID_REGISTER_REQUEST 1
#define MESSID_PLAYER_INPUT 2
#define MESSID_PLAYER_ATT_REQUEST 3

#define INVALID_PID 9999

#endif