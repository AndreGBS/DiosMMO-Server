keyUp = keyboard_check(vk_up);
keyDown = keyboard_check(vk_down);
keyLeft = keyboard_check(vk_left);
keyRight = keyboard_check(vk_right);

sendTimer--;

if(sendTimer <= 0)
{
	var buff = buffer_create(1024, buffer_grow, 1);
		buffer_seek(buff, buffer_seek_start, 0);
		buffer_write(buff, buffer_u8, MESSID_PLAYER_INPUT);
		buffer_write(buff, buffer_bool, keyRight);
		buffer_write(buff, buffer_bool, keyUp);
		buffer_write(buff, buffer_bool, keyLeft);
		buffer_write(buff, buffer_bool, keyDown);
	network_send_raw(global.connection, buff, buffer_tell(buff));
	
	sendTimer = 2;
}