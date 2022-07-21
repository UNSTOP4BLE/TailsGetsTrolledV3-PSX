/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "stage.h"
#include "mechanics.h"
#include "mutil.h"

int noteswitchcooldown;

static void ChangeNoteAngle(int noteangle);

void MessWithNotes()
{
	if (stage.song_beat == 36)
		Stage_MoveNote(0, 0, 1024);
	else if (stage.song_beat == 37)
		Stage_MoveNote(0, 0, -2048);
}	

static void ChangeNoteAngle(int noteangle)
{
	noteswitchcooldown ++;

	if (noteswitchcooldown == 1)
	{
		Stage_MoveNote(4, 0, noteangle - FIXED_DEC(2,1));
		Stage_MoveNote(5, 0, noteangle - FIXED_DEC(15,10));
		Stage_MoveNote(6, 0, noteangle - FIXED_DEC(10,10));
		Stage_MoveNote(7, 0, noteangle - FIXED_DEC(5,10));

		Stage_MoveNote(0, 0, noteangle + FIXED_DEC(5,10));
		Stage_MoveNote(1, 0, noteangle + FIXED_DEC(10,10));
		Stage_MoveNote(2, 0, noteangle + FIXED_DEC(15,10));
		Stage_MoveNote(3, 0, noteangle + FIXED_DEC(2,1));
	}
	else if (noteswitchcooldown == 30)
	{
		noteangle -= FIXED_DEC(1,1);
	}
}

