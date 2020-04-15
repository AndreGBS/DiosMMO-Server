

client = network_create_socket(network_socket_tcp);

conn = network_connect_raw(client, "25.131.217.177", 27015);

if(conn < 0)
	game_end();
	
