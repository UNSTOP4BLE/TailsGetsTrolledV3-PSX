/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "knuck.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//knuck character structure
enum
{
	knuck_ArcMain_Idle0,
	knuck_ArcMain_Idle1,
	knuck_ArcMain_Idle2,
	knuck_ArcMain_Idle3,
	knuck_ArcMain_Idle4,
	knuck_ArcMain_Idle5,
	knuck_ArcMain_Left0,
	knuck_ArcMain_Left1,
	knuck_ArcMain_Left2,
	knuck_ArcMain_Left3,
	knuck_ArcMain_Down0,
	knuck_ArcMain_Down1,
	knuck_ArcMain_Up0,
	knuck_ArcMain_Up1,
	knuck_ArcMain_Up2,
	knuck_ArcMain_Right0,
	knuck_ArcMain_Right1,
	
	knuck_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[knuck_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_knuck;

//knuck character definitions
static const CharFrame char_knuck_frame[] = {
	{knuck_ArcMain_Idle0, {  0,   0, 152, 143}, {  0,   0}},
	{knuck_ArcMain_Idle1, {  0,   0, 152, 139}, {  0,   0}},
	{knuck_ArcMain_Idle2, {  0,   0, 152, 140}, {  0,   0}},
	{knuck_ArcMain_Idle3, {  0,   0, 152, 141}, {  0,   0}},
	{knuck_ArcMain_Idle4, {  0,   0, 154, 143}, {  0,   0}},
	{knuck_ArcMain_Idle5, {  0,   0, 154, 144}, {  0,   0}},
	
	{knuck_ArcMain_Left0, {  0,   0, 182, 169}, {  0,   0}},
	{knuck_ArcMain_Left1, {  0,   0, 178, 174}, {  0,   0}},
	{knuck_ArcMain_Left2, {  0,   0, 170, 188}, {  0,   0}},
	{knuck_ArcMain_Left3, {  0,   0, 164, 186}, {  0,   0}},

	{knuck_ArcMain_Down0, {  0,   0, 150, 137}, {  0,   0}},
	{knuck_ArcMain_Down0, {105, 117, 151, 139}, {  0,   0}},
	{knuck_ArcMain_Down1, {  0,   0, 152, 139}, {  0,   0}}, 
	
	{knuck_ArcMain_Up0, {  0,   0, 138, 237}, {  0,   0}},
	{knuck_ArcMain_Up1, {  0,   0, 138, 245}, {  0,   0}}, 
	{knuck_ArcMain_Up2, {  0,   0, 134, 243}, {  0,   0}},
	
	{knuck_ArcMain_Right0, {  0,   0, 115, 160}, {  0,   0}},
	{knuck_ArcMain_Right0, {115,   0, 113, 160}, {  0,   0}}, 
	{knuck_ArcMain_Right1, {  0,   0, 112, 160}, {  0,   0}},
	{knuck_ArcMain_Right1, {112,   0, 112, 160}, {  0,   0}}, 
};

static const Animation char_knuck_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 6, 7, 8, 9, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 10, 11, 12, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 13, 14, 15, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 16, 17, 18, 19, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//knuck character functions
void Char_knuck_SetFrame(void *user, u8 frame)
{
	Char_knuck *this = (Char_knuck*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_knuck_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_knuck_Tick(Character *character)
{
	Char_knuck *this = (Char_knuck*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_knuck_SetFrame);
	Character_Draw(character, &this->tex, &char_knuck_frame[this->frame]);
}

void Char_knuck_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_knuck_Free(Character *character)
{
	Char_knuck *this = (Char_knuck*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_knuck_New(fixed_t x, fixed_t y)
{
	//Allocate knuck object
	Char_knuck *this = Mem_Alloc(sizeof(Char_knuck));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_knuck_New] Failed to allocate knuck object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_knuck_Tick;
	this->character.set_anim = Char_knuck_SetAnim;
	this->character.free = Char_knuck_Free;
	
	Animatable_Init(&this->character.animatable, char_knuck_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\KNUCK.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //knuck_ArcMain_Idle0
		"idle1.tim", //knuck_ArcMain_Idle1
		"idle2.tim", //knuck_ArcMain_Idle1
		"idle3.tim", //knuck_ArcMain_Idle1
		"idle4.tim", //knuck_ArcMain_Idle1
		"idle5.tim", //knuck_ArcMain_Idle1
		"left0.tim",  //knuck_ArcMain_Left
		"left1.tim",  //knuck_ArcMain_Left
		"left2.tim",  //knuck_ArcMain_Left
		"left3.tim",  //knuck_ArcMain_Left
		"down0.tim",  //knuck_ArcMain_Down
		"down1.tim",  //knuck_ArcMain_Down
		"up0.tim",    //knuck_ArcMain_Up
		"up1.tim",    //knuck_ArcMain_Up
		"up2.tim",    //knuck_ArcMain_Up
		"right0.tim", //knuck_ArcMain_Right
		"right1.tim", //knuck_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
