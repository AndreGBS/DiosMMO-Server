var mx = mouse_x;
var my = mouse_y;
if(point_in_rectangle(mx, my, bbox_left, bbox_top, bbox_right, bbox_bottom))
{
	image_index = 1;
	if(mouse_check_button_released(mb_left))
	{
		script_execute(actionScr);
	}
}
else
{
	image_index = 0;	
}
