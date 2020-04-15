var focusInput = mouse_check_button_released(mb_left) || keyboard_check_pressed(vk_enter);
var mx = mouse_x;
var my = mouse_y;

if(focusInput)
{
	focus = -999;
	with(objTextField)
	{
		if(point_in_rectangle(mx, my, bbox_left, bbox_top, bbox_right, bbox_bottom))
		{
			keyboard_string = other.fields[? fieldName];
			other.focus = id;
		}
	}
}

if(instance_exists(focus) && keyboard_check(vk_anykey))
{
	fields[? focus.fieldName] = keyboard_string;
}

