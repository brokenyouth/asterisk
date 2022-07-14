/*
	Asterisk
	Redouane Chabane (July, 2022)
	Player.cpp - Player logic.
*/

#include "Player.h"

#include "GameplayState.h"
#include "Level.h"
#include "Texture.h"

#include "Logger.h"

Player::Player(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking)
	: Actor(InGameplayState, InLevel, InPosition, IsBlocking)
{
	Health = 100;
}

Player::~Player()
{
}

void Player::Tick(float DeltaTime)
{
	glm::vec2 Direction = { glm::cos(CurrentRotation) , glm::sin(CurrentRotation) };
	glm::vec2 RightVector = { glm::cos(CurrentRotation + 3.1415 / 2.0f), glm::sin(CurrentRotation + 3.1415 / 2.0f) };
	
	Velocity += ForwardAxis * Direction;
	Velocity += RightAxis * RightVector;

	Velocity *= MoveSpeed * DeltaTime;

	Position += Velocity;

	/* Consume previous inputs */
	Velocity = { 0.0f, 0.0f };
	ForwardAxis = 0.0f;
	RightAxis = 0.0f;
}

void Player::MoveForward(float Value)
{
	ForwardAxis = Value;
}

void Player::MoveRight(float Value)
{
	RightAxis = Value;
}