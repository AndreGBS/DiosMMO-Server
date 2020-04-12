global.username = get_string("USERNAME", "");
global.password = get_string("PASSWORD", "");

client = network_create_socket(network_socket_tcp);

var conn = network_connect_raw(client, "25.131.217.177", 27015);

if(conn < 0)
	game_end();
	
var buff = buffer_create(1024, buffer_grow, 1);
buffer_seek(buff, buffer_seek_start, 0);
buffer_write(buff, buffer_u8, 0);
buffer_write(buff, buffer_string, global.username);
buffer_write(buff, buffer_string, global.password);
network_send_raw(conn, buff, buffer_tell(buff));
