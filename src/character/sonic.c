/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sonic.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//"sonic the chronic addict" - dreamcastnick

//Sonic character structure
enum
{
	Sonic_ArcMain_Normal0,
	Sonic_ArcMain_Normal1,
	Sonic_ArcMain_Normal2,
	Sonic_ArcMain_Normal3,
	Sonic_ArcMain_Normal4,
	Sonic_ArcMain_Normal5,
	Sonic_ArcMain_Normal6,

	Sonic_ArcMain_Mad0,
	Sonic_ArcMain_Mad1,
	Sonic_ArcMain_Mad2,
	Sonic_ArcMain_Mad3,
	Sonic_ArcMain_Mad4,
	Sonic_ArcMain_Mad5,
	Sonic_ArcMain_Mad6,

	Sonic_ArcMain_Forced0,
	Sonic_ArcMain_Forced1,
	Sonic_ArcMain_Forced2,
	Sonic_ArcMain_Forced3,
	Sonic_ArcMain_Forced4,
	Sonic_ArcMain_Forced5,
	Sonic_ArcMain_Forced6,
	Sonic_ArcMain_Forced7,
	
	Sonic_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Sonic_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Sonic;

//Sonic character definitions
static const CharFrame char_sonic_frame[] = {
	{Sonic_ArcMain_Normal0, {  2,   2, 106, 148}, { 58, 145}}, //0 idle 1
	{Sonic_ArcMain_Normal0, {110,   2, 105, 148}, { 57, 145}}, //1 idle 2
	{Sonic_ArcMain_Normal1, {  2,   2, 104, 153}, { 58, 149}}, //2 idle 3
	{Sonic_ArcMain_Normal1, {109,   2, 108, 156}, { 60, 153}}, //3 idle 4
	{Sonic_ArcMain_Normal2, {  1,   2, 107, 155}, { 60, 152}}, //4 idle 5
	
	{Sonic_ArcMain_Normal2, {112,   2, 106, 157}, { 58, 154}}, //5 left 1
	{Sonic_ArcMain_Normal3, {  3,   1, 107, 158}, { 58, 154}}, //6 left 2
	
	{Sonic_ArcMain_Normal3, {119,  12,  93, 147}, { 51, 144}}, //7 down 1
	{Sonic_ArcMain_Normal4, {  2,  10,  91, 149}, { 53, 146}}, //8 down 2
	
	{Sonic_ArcMain_Normal4, { 97,   1,  94, 163}, { 48, 160}}, //9 up 1
	{Sonic_ArcMain_Normal5, {  4,   3,  96, 159}, { 49, 156}}, //10 up 2
	
	{Sonic_ArcMain_Normal5, {102,   6,  87, 155}, { 43, 152}}, //11 right 1
	{Sonic_ArcMain_Normal6, {  3,   3,  83, 155}, { 43, 152}}, //12 right 2

	{Sonic_ArcMain_Mad0, {  1,   1,  93, 149}, { 59, 146}}, //13 idle 1
	{Sonic_ArcMain_Mad0, {108,   2,  92, 148}, { 59, 145}}, //14 idle 2
	{Sonic_ArcMain_Mad1, {  3,   2,  90, 152}, { 57, 149}}, //15 idle 3
	{Sonic_ArcMain_Mad1, {113,   4,  89, 155}, { 56, 151}}, //16 idle 4
	{Sonic_ArcMain_Mad2, {  6,   2,  89, 155}, { 55, 152}}, //17 idle 5
	
	{Sonic_ArcMain_Mad2, {110,   5, 100, 155}, { 60, 152}}, //18 left 1
	{Sonic_ArcMain_Mad3, {  3,   3, 102, 155}, { 58, 152}}, //19 left 2
	
	{Sonic_ArcMain_Mad3, {115,   9,  89, 150}, { 55, 147}}, //20 down 1
	{Sonic_ArcMain_Mad4, {  0,   7,  89, 152}, { 55, 149}}, //21 down 2
	
	{Sonic_ArcMain_Mad4, { 94,   7,  99, 157}, { 51, 154}}, //22 up 1
	{Sonic_ArcMain_Mad5, {  5,   3, 100, 156}, { 50, 154}}, //23 up 2
	
	{Sonic_ArcMain_Mad5, {111,   4, 107, 155}, { 45, 152}}, //24 right 1
	{Sonic_ArcMain_Mad6, {  5,   3, 108, 155}, { 48, 152}}, //25 right 2
};

static const Animation char_normal_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

static const Animation char_mad_anim[CharAnim_Max] = {
	{2, (const u8[]){13, 14, 15, 16, 17, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){18, 19, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){20, 21, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){22, 23, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){24, 25, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Sonic character functions
void Char_Sonic_SetFrame(void *user, u8 frame)
{
	Char_Sonic *this = (Char_Sonic*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_sonic_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Sonic_Tick(Character *character)
{
	Char_Sonic *this = (Char_Sonic*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Sonic_SetFrame);
	Character_Draw(character, &this->tex, &char_sonic_frame[this->frame]);
}

void Char_Sonic_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Sonic_Free(Character *character)
{
	Char_Sonic *this = (Char_Sonic*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Sonic_New(fixed_t x, fixed_t y)
{
	//Allocate sonic object
	Char_Sonic *this = Mem_Alloc(sizeof(Char_Sonic));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Sonic_New] Failed to allocate sonic object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Sonic_Tick;
	this->character.set_anim = Char_Sonic_SetAnim;
	this->character.free = Char_Sonic_Free;
	
	Animatable_Init(&this->character.animatable, char_normal_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(25,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SONIC.ARC;1");
	
	const char **pathp = (const char *[]){
		"normal0.tim", //Sonic_ArcMain_Normal
		"normal1.tim", //Sonic_ArcMain_Normal
		"normal2.tim", //Sonic_ArcMain_Normal
		"normal3.tim", //Sonic_ArcMain_Normal
		"normal4.tim", //Sonic_ArcMain_Normal
		"normal5.tim", //Sonic_ArcMain_Normal
		"normal6.tim", //Sonic_ArcMain_Normal

		"mad0.tim", //Sonic_ArcMain_Mad
		"mad1.tim", //Sonic_ArcMain_Mad
		"mad2.tim", //Sonic_ArcMain_Mad
		"mad3.tim", //Sonic_ArcMain_Mad
		"mad4.tim", //Sonic_ArcMain_Mad
		"mad5.tim", //Sonic_ArcMain_Mad
		"mad6.tim", //Sonic_ArcMain_Mad

		"forced0.tim", //Sonic_ArcMain_Forced
		"forced1.tim", //Sonic_ArcMain_Forced
		"forced2.tim", //Sonic_ArcMain_Forced
		"forced3.tim", //Sonic_ArcMain_Forced
		"forced4.tim", //Sonic_ArcMain_Forced
		"forced5.tim", //Sonic_ArcMain_Forced
		"forced6.tim", //Sonic_ArcMain_Forced
		"forced7.tim", //Sonic_ArcMain_Forced
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
