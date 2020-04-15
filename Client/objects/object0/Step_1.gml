var r = keyboard_check_pressed(ord("R"));
var l = keyboard_check_pressed(ord("L"))

if(r)
{
	global.username = get_string("USERNAME", "");
	global.password = get_string("PASSWORD", "");

	var buff = buffer_create(1024, buffer_grow, 1);
	buffer_seek(buff, buffer_seek_start, 0);
	buffer_write(buff, buffer_u8, 1);
	buffer_write(buff, buffer_string, global.username);
	buffer_write(buff, buffer_string, md5_string_utf8(global.password));
	network_send_raw(conn, buff, buffer_tell(buff));
}
else if(l)
{
	global.username = get_string("USERNAME", "");
	global.password = get_string("PASSWORD", "");

	var buff = buffer_create(1024, buffer_grow, 1);
	buffer_seek(buff, buffer_seek_start, 0);
	buffer_write(buff, buffer_u8, 0);
	buffer_write(buff, buffer_string, global.username);
	buffer_write(buff, buffer_string, md5_string_utf8(global.password));
	network_send_raw(conn, buff, buffer_tell(buff));	
}