/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "tails.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//tails character structure
enum
{
	tails_ArcMain_Idle0,
	tails_ArcMain_Idle1,
	tails_ArcMain_Left,
	tails_ArcMain_Down,
	tails_ArcMain_Up,
	tails_ArcMain_Right,
	
	tails_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[tails_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_tails;

//tails character definitions
static const CharFrame char_tails_frame[] = {
	{tails_ArcMain_Idle0, {  0,   0, 118, 151}, { 44, 151}}, //0 idle 1
	{tails_ArcMain_Idle0, {118,   0, 116, 151}, { 42, 149}}, //1 idle 2
	{tails_ArcMain_Idle1, {  0,   0, 112, 160}, { 40, 152}}, //2 idle 3
	{tails_ArcMain_Idle1, {112,   0, 108, 160}, { 38, 160}}, //3 idle 4
	
	{tails_ArcMain_Left, {  0,   0, 114, 159}, { 40, 158}}, //4 left 1
	{tails_ArcMain_Left, {114,   0, 114, 159}, { 39, 158}}, //5 left 2
	
	{tails_ArcMain_Down, {  0,   0, 106, 157}, { 36, 156}}, //6 down 1
	{tails_ArcMain_Down, {106,   0, 110, 157}, { 39, 157}}, //7 down 2
	
	{tails_ArcMain_Up, {  0,   0, 105, 164}, { 35, 163}}, //8 up 1
	{tails_ArcMain_Up, {105,   0, 105, 164}, { 34, 162}}, //9 up 2
	
	{tails_ArcMain_Right, {  0,   0, 117, 152}, { 43, 150}}, //10 right 1
	{tails_ArcMain_Right, {117,   0, 117, 152}, { 41, 151}}, //11 right 2
};

static const Animation char_tails_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 6,  7, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 8,  9, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){10, 11, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//tails character functions
void Char_tails_SetFrame(void *user, u8 frame)
{
	Char_tails *this = (Char_tails*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_tails_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_tails_Tick(Character *character)
{
	Char_tails *this = (Char_tails*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_tails_SetFrame);
	Character_Draw(character, &this->tex, &char_tails_frame[this->frame]);
}

void Char_tails_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_tails_Free(Character *character)
{
	Char_tails *this = (Char_tails*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_tails_New(fixed_t x, fixed_t y)
{
	//Allocate tails object
	Char_tails *this = Mem_Alloc(sizeof(Char_tails));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_tails_New] Failed to allocate tails object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_tails_Tick;
	this->character.set_anim = Char_tails_SetAnim;
	this->character.free = Char_tails_Free;
	
	Animatable_Init(&this->character.animatable, char_tails_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(45,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\TAILS.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //tails_ArcMain_Idle0
		"idle1.tim", //tails_ArcMain_Idle1
		"left.tim",  //tails_ArcMain_Left
		"down.tim",  //tails_ArcMain_Down
		"up.tim",    //tails_ArcMain_Up
		"right.tim", //tails_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
