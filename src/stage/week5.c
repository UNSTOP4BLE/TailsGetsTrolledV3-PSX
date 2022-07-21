/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week5.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"

//Week 5 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; 
	Gfx_Tex tex_back1;
} Back_Week5;

//Week 5 background functions
void Back_Week5_DrawBG(StageBack *back)
{
	Back_Week5 *this = (Back_Week5*)back;
	
	fixed_t fx, fy;
	
	//Draw bg
	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT bg_src[3] = {
		{  0, 124, 249, 132},
		{  0,   0, 256, 124},
		{181,   0,  75, 124} // tree
	};
	RECT_FIXED bg_dst[3] = {
		{FIXED_DEC(-311,1) - fx, FIXED_DEC(17,1) - fy, FIXED_DEC(541,1), FIXED_DEC(132,1)},
		{FIXED_DEC(-275,1) - stage.camera.x * 7 / 10, FIXED_DEC(-5,1) - stage.camera.y * 7 / 10, FIXED_DEC(576,1), FIXED_DEC(124,1)},
		{FIXED_DEC(84,1) - fx, FIXED_DEC(-107,1) - fy, FIXED_DEC(162,1), FIXED_DEC(124,1)} //tree
	};
		
	if (stage.widescreen)
	{
		bg_dst[0].x = FIXED_DEC(-343,1) - fx; 
		bg_dst[0].w = FIXED_DEC(642,1);	

		bg_dst[1].x = FIXED_DEC(-366,1) - stage.camera.x * 7 / 10; 
		bg_dst[1].w = FIXED_DEC(702,1);	

		bg_dst[2].x = FIXED_DEC(127,1) - fx; 
		bg_dst[2].w = FIXED_DEC(188,1);	
	}
	Debug_StageMoveDebug(&bg_dst[0], 4, fx, fy);
	Debug_StageMoveDebug(&bg_dst[1], 5, stage.camera.x * 7 / 10, stage.camera.y * 7 / 10);
	Debug_StageMoveDebug(&bg_dst[2], 6, fx, fy);


	Stage_DrawTex(&this->tex_back1, &bg_src[0], &bg_dst[0], stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back0, &bg_src[2], &bg_dst[2], stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back1, &bg_src[1], &bg_dst[1], stage.camera.bzoom);

	//Draw sky
	fx = stage.camera.x * 3 / 10;
	fy = stage.camera.y * 3 / 10;

	RECT sky_src = {0, 0, 181, 256};
	RECT_FIXED sky_dst = {FIXED_DEC(-186,1) - fx, FIXED_DEC(-120,1) - fy, FIXED_DEC(365,1), FIXED_DEC(252,1)};
		
	if (stage.widescreen)
	{
		sky_dst.x = FIXED_DEC(-283,1) - fx; 
		sky_dst.w = FIXED_DEC(563,1);	
	}

	Debug_StageMoveDebug(&sky_dst, 7, fx, fy);
	Stage_DrawTex(&this->tex_back0, &sky_src, &sky_dst, stage.camera.bzoom);
}

void Back_Week5_Free(StageBack *back)
{
	Back_Week5 *this = (Back_Week5*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week5_New(void)
{
	//Allocate background structure
	Back_Week5 *this = (Back_Week5*)Mem_Alloc(sizeof(Back_Week5));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week5_DrawBG;
	this->back.free = Back_Week5_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\CH5\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
