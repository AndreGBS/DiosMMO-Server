CC=g++
CFLAGS=-std=c++17 -lws2_32 

edit: Server.o Logger.o Client.o
	$(CC) -o .\Bin\x64\Server .\Src\Main.cpp .\Obj\Server.o .\Obj\Logger.o .\Obj\Client.o $(CFLAGS)

Server.o: 
	$(CC) -o .\Obj\Server.o -c .\Src\Server.cpp $(CFLAGS)

Client.o:
	$(CC) -o .\Obj\Client.o -c .\Src\Client.cpp $(CFLAGS)

Logger.o:
	$(CC) -o .\Obj\Logger.o -c .\Src\Logger.cpp $(CFLAGS)

clean: 
	del .\Obj\*.o