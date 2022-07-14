/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week1.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"

//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Stage and back
	Gfx_Tex tex_back1; //Curtains
} Back_Week1;

//Week 1 background functions
void Back_Week1_DrawBG(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	fixed_t fx, fy;
	
	//Draw curtains
	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT tree_src = {0, 0, 74, 256};
	RECT_FIXED tree_dst = {FIXED_DEC(0,1) - fx, FIXED_DEC(0,1) - fy, FIXED_DEC(tree_src.w,1), FIXED_DEC(tree_src.h,1)}; 
	
	Stage_DrawTex(&this->tex_back0, &tree_src, &tree_dst, stage.camera.bzoom);
	tree_src.x = 74;
	tree_src.w = 62;
	tree_dst.w = FIXED_DEC(tree_src.w,1);
	Stage_DrawTex(&this->tex_back0, &tree_src, &tree_dst, stage.camera.bzoom);
}

void Back_Week1_Free(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week1_New(void)
{
	//Allocate background structure
	Back_Week1 *this = (Back_Week1*)Mem_Alloc(sizeof(Back_Week1));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week1_DrawBG;
	this->back.free = Back_Week1_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK1\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Mem_Free(arc_back);
	
	Gfx_SetClear(154, 217, 234);

	return (StageBack*)this;
}
