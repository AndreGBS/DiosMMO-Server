var username = objUIManager.fields[? "username"];
var password = objUIManager.fields[? "password"];

if(string_length(username) < 1)
{
	show_message("O campo username esta vazio");
	exit;
}

if(string_length(username) > 24)
{
	show_message("O campo username nao pode conter mais de 24 caracteres");
	exit;
}

if(string_length(password) < 1)
{
	show_message("O campo password esta vazio");
	exit;
}

if(string_length(password) > 18)
{
	show_message("O campo password nao pode conter mais de 18 caracteres");
	exit;
}

var buff = buffer_create(1024, buffer_grow, 1);
	buffer_seek(buff, buffer_seek_start, 0);
	buffer_write(buff, buffer_u8, 0);
	buffer_write(buff, buffer_string, username);
	buffer_write(buff, buffer_string, md5_string_utf8(password));
network_send_raw(global.connection, buff, buffer_tell(buff));

