attTimer--;

if(attTimer <= 0)
{
	var buff = buffer_create(1024, buffer_grow, 1);
		buffer_seek(buff, buffer_seek_start, 0);
		buffer_write(buff, buffer_u8, MESSID_PLAYER_ATT_REQUEST);
	network_send_raw(global.connection, buff, buffer_tell(buff));
	
	attTimer = 0;	
}