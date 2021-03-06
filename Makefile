CC=g++
CFLAGS=-std=c++17 -O3 -I"./Include" -lpthread -lpqxx -lpq 

edit: Server.o Logger.o Client.o UpdateManager.o Buffer.o Entity.o
	$(CC) -o ./Bin/x64/Server ./Src/Main.cpp ./Obj/Server.o ./Obj/Logger.o ./Obj/Client.o ./Obj/UpdateManager.o ./Obj/Buffer.o $(CFLAGS)

Server.o: 
	$(CC) -o ./Obj/Server.o -c ./Src/Server.cpp $(CFLAGS)

Client.o:
	$(CC) -o ./Obj/Client.o -c ./Src/Client.cpp $(CFLAGS)

Logger.o:
	$(CC) -o ./Obj/Logger.o -c ./Src/Logger.cpp $(CFLAGS)

UpdateManager.o:
	$(CC) -o ./Obj/UpdateManager.o -c ./Src/UpdateManager.cpp $(CFLAGS)

Buffer.o:
	$(CC) -o ./Obj/Buffer.o -c ./Src/Buffer.cpp $(CFLAGS)

Entity.o:
	$(CC) -o ./Obj/Entity.o -c ./Src/Entity.cpp $(CFLAGS)

clean: 
	del ./Obj/*.o