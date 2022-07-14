/*
	Asterisk
	Redouane Chabane (July, 2022)
	State.cpp - Base state class, inherited by other actual states like MainMenu, Gameplay, EndGame, etc..
*/

#include "State.h"

State::State(Game& pGame)
	: mGame(pGame)
{

}
