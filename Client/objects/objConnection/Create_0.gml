client = network_create_socket(network_socket_tcp);
global.connection = network_connect_raw(client, "25.131.217.177", 27015);

if(global.connection < 0)
	game_end();
	
