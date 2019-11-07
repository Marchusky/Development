// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	// TODO 1: Fill Start(), Read(), ReadSec() methods
	// they are simple, one line each!
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	uint time_passed = SDL_GetTicks() - started_at;
	return 0;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	uint time_passed_s = (SDL_GetTicks() - started_at) / 1000;
	return 0.0f;
}