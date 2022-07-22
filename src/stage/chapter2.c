/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "chapter2.h"

#include "../mem.h"
#include "../archive.h"

//Chapter 2 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Background
	Gfx_Tex tex_cloud; //Cloud
} Back_Chapter2;

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
	RECT_FIXED green_dst = {
	FIXED_DEC(-screen.SCREEN_WIDTH / 2,1), 
	grass_dst.y + FIXED_DEC(11,1), 
	FIXED_DEC(screen.SCREEN_WIDTH,1), 
	FIXED_DEC(120,1)
};
	Stage_DrawTex(&tex, &green_fill, &green_dst, stage.camera.bzoom);
}

//Chapter 2 background functions
void Back_Chapter2_DrawBG(StageBack *back)
{
	Back_Chapter2 *this = (Back_Chapter2*)back;
	
	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT plant_src = {150, 49, 68, 50};
	RECT plant2_src = {177, 124, 17, 36};

	RECT_FIXED plant_dst = {
	FIXED_DEC(-196,1) - fx, 
	FIXED_DEC(88,1) - fy, 
	FIXED_DEC(68 * 2,1),
	 FIXED_DEC(50 *2,1)
};

	if (stage.widescreen)
	plant_dst.x -= FIXED_DEC(100,1);

	Stage_DrawTex(&this->tex_back0, &plant_src, &plant_dst, stage.camera.bzoom);

	RECT_FIXED plant2_dst = {
	plant_dst.x + FIXED_DEC(280,1), 
	plant_dst.y, 
	FIXED_DEC(17 *2,1),
	 FIXED_DEC(36 * 2,1)
};
	if (stage.widescreen)
	plant2_dst.x += FIXED_DEC(172,1);

	Stage_DrawTex(&this->tex_back0, &plant2_src, &plant2_dst, stage.camera.bzoom);

	//Draw bg
	fx = stage.camera.x * 13 / 10;
	fy = stage.camera.y * 13 / 10;

	RECT_FIXED grass_dst = {
	FIXED_DEC(-229,1) - stage.camera.x, 
	FIXED_DEC(44,1) - stage.camera.y, 
	FIXED_DEC(205,1), 
	FIXED_DEC(11,1)
}; 

	RECT tree_src = {0, 0, 74, 256};
	RECT_FIXED tree_dst = {
	FIXED_DEC(-266,1) - fx, 
	FIXED_DEC(-144,1) - fy, 
	FIXED_DEC(tree_src.w,1), 
	FIXED_DEC(tree_src.h,1)
}; 
	
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

	fx = stage.camera.x;
	fy = stage.camera.y;

	//draw clouds
	RECT cloud1_src = {2, 6, 193, 75};
	RECT_FIXED cloud1_dst = {
	FIXED_DEC(-51,1) - fx, 
	FIXED_DEC(-117,1) - fy, 
	FIXED_DEC(199,1),
	FIXED_DEC(97,1)
	};

	if (stage.widescreen)
		cloud1_dst.x = FIXED_DEC(55,1) - fx;
	
	Debug_StageMoveDebug(&cloud1_dst, 5, stage.camera.x, stage.camera.y);
	Stage_DrawTex(&this->tex_cloud, &cloud1_src, &cloud1_dst, stage.camera.bzoom);

	RECT cloud2_src = {25,105, 142, 44};
	RECT_FIXED cloud2_dst = {
	FIXED_DEC(-281,1) - fx, 
	FIXED_DEC(-84,1) - fy, 
	FIXED_DEC(142,1),
	FIXED_DEC(44,1)
	};

	if (stage.widescreen)
		cloud2_dst.x = FIXED_DEC(-368,1) - fx;
	
	Debug_StageMoveDebug(&cloud2_dst, 6, stage.camera.x, stage.camera.y);
	Stage_DrawTex(&this->tex_cloud, &cloud2_src, &cloud2_dst, stage.camera.bzoom);
}

void Back_Chapter2_Free(StageBack *back)
{
	Back_Chapter2 *this = (Back_Chapter2*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Chapter2_New(void)
{
	//Allocate background structure
	Back_Chapter2 *this = (Back_Chapter2*)Mem_Alloc(sizeof(Back_Chapter2));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Chapter2_DrawBG;
	this->back.free = Back_Chapter2_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\CH2\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_cloud, Archive_Find(arc_back, "cloud.tim"), 0);
	Mem_Free(arc_back);

	Gfx_SetClear(154, 217, 234);
	
	return (StageBack*)this;
}
