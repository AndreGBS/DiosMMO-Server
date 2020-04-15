if(objUIManager.focus == id)
{
	image_index = 1;
}
else
{
	image_index = 0;	
}
draw_self();

if(!ds_map_exists(objUIManager.fields, fieldName))
{
	objUIManager.fields[? fieldName] = "";
}

draw_set_font(font0);
draw_set_valign(fa_middle);
draw_text(x+textXOffSet, y+(sprite_height*image_yscale)/2, objUIManager.fields[? fieldName]);