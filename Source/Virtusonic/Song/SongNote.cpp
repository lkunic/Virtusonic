// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongNote.h"

/*
 * Initializes the note structure. Used instead of the constructor when instantiating the note.
 */
void USongNote::Init(int32 startTick, int32 endTick, int32 pitch, int32 velocity)
{
	StartTick = startTick;
	_endTick = endTick;
	_pitch = pitch;
	_velocity = velocity;
}

/*
 * Returns the note start tick.
 */
int32 USongNote::GetStartTick()
{
	return StartTick;
}

/*
* Returns the note end tick.
*/
int32 USongNote::GetEndTick()
{
	return _endTick;
}

/*
* Returns the note pitch.
*/
int32 USongNote::GetPitch()
{
	return _pitch;
}

/*
* Returns the note velocity.
*/
int32 USongNote::GetVelocity()
{
	return _velocity;
}

/*
 * Returns the actual note frequency calculated using the pitch identifier.
 * Assumes A4 note on 440Hz.
 */
double USongNote::GetFrequency()
{
	return 440.0f * pow(NOTE_FREQUENCY_FACTOR, _pitch - 0x45);
}

/*
* Sets the note end tick.
*/
void USongNote::SetEndTick(int32 endTick)
{
	_endTick = endTick;
}


