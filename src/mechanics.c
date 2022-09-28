/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "stage.h"
#include "mechanics.h"
#include "mutil.h"
#include "timer.h"

int notex;
int bumptable[2][4] = {
	{1, 2, -1, -2},
	{-1, -2, 1, 2}
};
static void ChangeNoteAngle(int notex);
static boolean bumpnote(boolean left);

void MessWithNotes()
{
	if (timer_dt/23 == 20 || timer_dt/23 == 21 || timer_dt/23 == 22 || timer_dt/23 == 23 || timer_dt/23 == 24)
		bumpnote(false);
}	

static void ChangeNoteAngle(int notex)
{
	
	Stage_MoveNote(4, notex, 0);
	Stage_MoveNote(5, notex, 0);
	Stage_MoveNote(6, notex, 0);
	Stage_MoveNote(7, notex, 0);

	Stage_MoveNote(0, notex, 0);
	Stage_MoveNote(1, notex, 0);
	Stage_MoveNote(2, notex, 0);
	Stage_MoveNote(3, notex, 0);
}

static boolean bumpnote(boolean left)
{	
	for (int i = 0; i < 4; i++)
	{
		Stage_MoveNote(0, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(1, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(2, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(3, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(4, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(5, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(6, FIXED_DEC(bumptable[left][i], 1), 0);
		Stage_MoveNote(7, FIXED_DEC(bumptable[left][i], 1), 0);
		if (i == 3) 
			return true;
	}
}