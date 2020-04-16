var buff = async_load[? "buffer"];
var buffSize = async_load[? "size"];
do
{
	var messid = buffer_read(buff, buffer_u8);

	switch(messid)
	{
		case 0:
			show_message("Login :" + string(buffer_read(buff, buffer_u8)));
		break;
	
		case 1:
			show_message("Register :" + string(buffer_read(buff, buffer_u8)));
		break;
	
		case MESSID_PLAYER_ATT_REQUEST:
			if(!instance_exists(objPlayer))
				instance_create_layer(0, 0, "Instances", objPlayer);
			objPlayer.x = buffer_read(buff, buffer_s32);
			objPlayer.y = buffer_read(buff, buffer_s32);
		break;
	}
}until(buffer_tell(buff) >= buffSize);