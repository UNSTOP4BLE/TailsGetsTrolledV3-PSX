/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "darksonic.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//darksonic character structure
enum
{
	darksonic_ArcMain_Idle0,
	darksonic_ArcMain_Idle1,
	darksonic_ArcMain_Idle2,
	darksonic_ArcMain_Idle3,
	darksonic_ArcMain_Idle4,
	darksonic_ArcMain_Idle5,
	darksonic_ArcMain_Idle6,
	darksonic_ArcMain_Idle7,
	darksonic_ArcMain_Idle8,
	darksonic_ArcMain_Idle9,
	darksonic_ArcMain_Idle10,
	darksonic_ArcMain_Left0,
	darksonic_ArcMain_Left1,
	darksonic_ArcMain_Left2,
	darksonic_ArcMain_Left3,
	darksonic_ArcMain_Down0,
	darksonic_ArcMain_Down1,
	darksonic_ArcMain_Down2,
	darksonic_ArcMain_Down3,
	darksonic_ArcMain_Up0,
	darksonic_ArcMain_Up1,
	darksonic_ArcMain_Right0,
	darksonic_ArcMain_Right1,
	darksonic_ArcMain_Right2,
	darksonic_ArcMain_Right3,
	
	darksonic_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[darksonic_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_darksonic;

//darksonic character definitions
static const CharFrame char_darksonic_frame[] = {
	{darksonic_ArcMain_Idle0, {  0,   0, 156, 147}, { 51, 147}}, 
	{darksonic_ArcMain_Idle1, {  0,   0, 156, 143}, { 50, 143}}, 
	{darksonic_ArcMain_Idle2, {  0,   0, 156, 147}, { 51, 147}}, 
	{darksonic_ArcMain_Idle3, {  0,   0, 156, 144}, { 51, 143}}, 
	{darksonic_ArcMain_Idle4, {  0,   0, 152, 152}, { 51, 151}}, 
	{darksonic_ArcMain_Idle5, {  0,   0, 152, 152}, { 51, 151}}, 
	{darksonic_ArcMain_Idle6, {  0,   0, 152, 148}, { 51, 147}}, 
	{darksonic_ArcMain_Idle7, {  0,   0, 152, 156}, { 51, 155}}, 
	{darksonic_ArcMain_Idle8, {  0,   0, 152, 153}, { 51, 152}}, 	
	{darksonic_ArcMain_Idle9, {  0,   0, 152, 156}, { 51, 155}}, 
	{darksonic_ArcMain_Idle10, {  0,   0, 152, 154}, { 51, 153}}, 

	{darksonic_ArcMain_Left0, {  0,   0, 164, 143}, { 54, 142}}, 
	{darksonic_ArcMain_Left1, {  0,   0, 168, 139}, { 54, 138}}, 
	{darksonic_ArcMain_Left2, {  0,   0, 164, 144}, { 48, 144}}, 
	{darksonic_ArcMain_Left3, {  0,   0, 164, 141}, { 48, 140}}, 
	
	{darksonic_ArcMain_Down0, {  0,   0, 160, 140}, { 54, 139}}, 
	{darksonic_ArcMain_Down1, {  0,   0, 160, 138}, { 54, 137}}, 
	{darksonic_ArcMain_Down2, {  0,   0, 156, 143}, { 52, 142}}, 
	{darksonic_ArcMain_Down3, {  0,   0, 160, 140}, { 53, 139}}, 
	
	{darksonic_ArcMain_Up0, {  0,   0, 127, 181}, { 51, 180}}, 
	{darksonic_ArcMain_Up0, {127,   0, 127, 188}, { 50, 188}}, 
	{darksonic_ArcMain_Up1, {  0,   0, 122, 187}, { 46, 180}}, 
	{darksonic_ArcMain_Up1, {122,   0, 122, 187}, { 46, 187}}, 

	{darksonic_ArcMain_Right0, {  0,   0, 136, 148}, { 46, 148}}, 
	{darksonic_ArcMain_Right1, {  0,   0, 136, 146}, { 46, 146}},  
    {darksonic_ArcMain_Right2, {  0,   0, 132, 148}, { 45, 148}}, 
	{darksonic_ArcMain_Right3, {  0,   0, 132, 146}, { 45, 146}},  
};

static const Animation char_darksonic_anim[CharAnim_Max] = {
	{1, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 10, 9, 10, ASCR_CHGANI, CharAnim_Idle}}, //CharAnim_Idle
	{2, (const u8[]){ 11, 12, 13, 14, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 15, 16, 17, 18, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 19, 20, 21, 22, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 23, 24, 25, 26, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//darksonic character functions
void Char_darksonic_SetFrame(void *user, u8 frame)
{
	Char_darksonic *this = (Char_darksonic*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_darksonic_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_darksonic_Tick(Character *character)
{
	Char_darksonic *this = (Char_darksonic*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_darksonic_SetFrame);
	Character_Draw(character, &this->tex, &char_darksonic_frame[this->frame]);
}

void Char_darksonic_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_darksonic_Free(Character *character)
{
	Char_darksonic *this = (Char_darksonic*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_darksonic_New(fixed_t x, fixed_t y)
{
	//Allocate darksonic object
	Char_darksonic *this = Mem_Alloc(sizeof(Char_darksonic));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_darksonic_New] Failed to allocate darksonic object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_darksonic_Tick;
	this->character.set_anim = Char_darksonic_SetAnim;
	this->character.free = Char_darksonic_Free;
	
	Animatable_Init(&this->character.animatable, char_darksonic_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(45,1);
	this->character.focus_y = FIXED_DEC(23 - 96,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SONDARK.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //darksonic_ArcMain_Idle0
		"idle1.tim", //darksonic_ArcMain_Idle1
		"idle2.tim", //darksonic_ArcMain_Idle1
		"idle3.tim", //darksonic_ArcMain_Idle1
		"idle4.tim", //darksonic_ArcMain_Idle1
		"idle5.tim", //darksonic_ArcMain_Idle1
		"idle6.tim", //darksonic_ArcMain_Idle1
		"idle7.tim", //darksonic_ArcMain_Idle1
		"idle8.tim", //darksonic_ArcMain_Idle1
		"idle9.tim", //darksonic_ArcMain_Idle1
		"idle10.tim", 
		"left0.tim",  //darksonic_ArcMain_Left
		"left1.tim",  //darksonic_ArcMain_Left
		"left2.tim",  //darksonic_ArcMain_Left
		"left3.tim",  //darksonic_ArcMain_Left
		"down0.tim",  //darksonic_ArcMain_Down
		"down1.tim",  //darksonic_ArcMain_Down
		"down2.tim",  //darksonic_ArcMain_Down
		"down3.tim",  //darksonic_ArcMain_Down
		"up0.tim",    //darksonic_ArcMain_Up
		"up1.tim",    //darksonic_ArcMain_Up
		"right0.tim", //darksonic_ArcMain_Right
		"right1.tim", //darksonic_ArcMain_Right
		"right2.tim", //darksonic_ArcMain_Right
		"right3.tim", //darksonic_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
