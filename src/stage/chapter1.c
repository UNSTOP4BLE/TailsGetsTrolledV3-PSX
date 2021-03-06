/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "chapter1.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"

//Chapter 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Stage and back
} Back_Chapter1;

static void DrawGrass(Gfx_Tex tex, RECT_FIXED grass_dst)
{
	RECT grass_src = {136, 0, 119, 11};
	Stage_DrawTex(&tex, &grass_src, &grass_dst, stage.camera.bzoom);
	grass_src.y += 11;
	grass_dst.x += grass_dst.w;
	Stage_DrawTex(&tex, &grass_src, &grass_dst, stage.camera.bzoom);
	grass_src.y += 11;
	grass_dst.x += grass_dst.w;
	Stage_DrawTex(&tex, &grass_src, &grass_dst, stage.camera.bzoom);

	RECT green_fill = {152, 6, 1, 1};
	RECT_FIXED green_dst = {FIXED_DEC(-screen.SCREEN_WIDTH / 2,1), grass_dst.y + FIXED_DEC(11,1), FIXED_DEC(screen.SCREEN_WIDTH,1), FIXED_DEC(120,1)};
	Stage_DrawTex(&tex, &green_fill, &green_dst, stage.camera.bzoom);
}

//Chapter 1 background functions
void Back_Chapter1_DrawBG(StageBack *back)
{
	Back_Chapter1 *this = (Back_Chapter1*)back;
	
	fixed_t fx, fy;

	//Draw bg
	fx = stage.camera.x * 13 / 10;
	fy = stage.camera.y * 13 / 10;

	RECT_FIXED grass_dst = {FIXED_DEC(-229,1) - stage.camera.x, FIXED_DEC(44,1) - stage.camera.y, FIXED_DEC(205,1), FIXED_DEC(11,1)}; 
	RECT tree_src = {0, 0, 74, 256};
	RECT_FIXED tree_dst = {FIXED_DEC(-266,1) - fx, FIXED_DEC(-144,1) - fy, FIXED_DEC(tree_src.w,1), FIXED_DEC(tree_src.h,1)}; 
	
	Debug_StageMoveDebug(&grass_dst, 4, stage.camera.x, stage.camera.y);
	if (stage.widescreen)
	{
		tree_dst.x = FIXED_DEC(-357,1) - fx;
		grass_dst.x = FIXED_DEC(-305,1) - stage.camera.x;
		grass_dst.w = FIXED_DEC(263,1);
	}
	Stage_DrawTex(&this->tex_back0, &tree_src, &tree_dst, stage.camera.bzoom);
	tree_src.x = 74;
	tree_src.w = 62;
	tree_dst.w = FIXED_DEC(tree_src.w,1);
	tree_dst.x = FIXED_DEC(136,1) - fx;
	tree_dst.y = FIXED_DEC(-87,1) - fy;
	if (stage.widescreen)
		tree_dst.x = FIXED_DEC(235,1) - fx;
	Stage_DrawTex(&this->tex_back0, &tree_src, &tree_dst, stage.camera.bzoom);
	DrawGrass(this->tex_back0, grass_dst);
}

void Back_Chapter1_Free(StageBack *back)
{
	Back_Chapter1 *this = (Back_Chapter1*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Chapter1_New(void)
{
	//Allocate background structure
	Back_Chapter1 *this = (Back_Chapter1*)Mem_Alloc(sizeof(Back_Chapter1));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Chapter1_DrawBG;
	this->back.free = Back_Chapter1_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\CH1\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Mem_Free(arc_back);
	
	Gfx_SetClear(154, 217, 234);

	return (StageBack*)this;
}
