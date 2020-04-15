var buff = async_load[? "buffer"];
var messid = buffer_read(buff, buffer_u8);

switch(messid)
{
	case 0:
		show_message("Login :" + string(buffer_read(buff, buffer_u8)));
	break;
	
	case 1:
		show_message("Register :" + string(buffer_read(buff, buffer_u8)));
	break;
}