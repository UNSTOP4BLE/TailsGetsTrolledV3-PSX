/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week4.h"

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
} Back_Week4;

//Week 1 background functions
void Back_Week4_DrawBG(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;

	//Draw bg
	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT front_src = {0, 175, 255, 81};
	RECT_FIXED front_dst = {FIXED_DEC(0,1) - fx, FIXED_DEC(0,1) - fy, FIXED_DEC(255,1), FIXED_DEC(81,1)};

	fx = stage.camera.x * 4 / 10;
	fy = stage.camera.y * 4 / 10;

	RECT back_src = {0, 0, 255, 174};
	RECT_FIXED back_dst = {FIXED_DEC(0,1) - fx, FIXED_DEC(0,1) - fy, FIXED_DEC(255,1), FIXED_DEC(174,1)};

	Stage_DrawTex(&this->tex_back0, &back_src, &back_dst, stage.camera.bzoom);
}

void Back_Week4_Free(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week4_New(void)
{
	//Allocate background structure
	Back_Week4 *this = (Back_Week4*)Mem_Alloc(sizeof(Back_Week4));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week4_DrawBG;
	this->back.free = Back_Week4_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\CH4\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Mem_Free(arc_back);
	
	Gfx_SetClear(154, 217, 234);

	return (StageBack*)this;
}
