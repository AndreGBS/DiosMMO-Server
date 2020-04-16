CC=g++
CFLAGS=-std=c++17 -I".\Include" -lpqxx -lpq -lws2_32 

edit: Server.o Logger.o Client.o UpdateManager.o
	$(CC) -o .\Bin\x64\Server .\Src\Main.cpp .\Obj\Server.o .\Obj\Logger.o .\Obj\Client.o .\Obj\UpdateManager.o $(CFLAGS)

Server.o: 
	$(CC) -o .\Obj\Server.o -c .\Src\Server.cpp $(CFLAGS)

Client.o:
	$(CC) -o .\Obj\Client.o -c .\Src\Client.cpp $(CFLAGS)

Logger.o:
	$(CC) -o .\Obj\Logger.o -c .\Src\Logger.cpp $(CFLAGS)

UpdateManager.o:
	$(CC) -o .\Obj\UpdateManager.o -c .\Src\UpdateManager.cpp $(CFLAGS)
clean: 
	del .\Obj\*.o