/*
	Asterisk
	Redouane Chabane (July, 2022)
	Player.cpp - Player logic.
*/

#include "Player.h"

#include "GameplayState.h"
#include "Level.h"
#include "Texture.h"
#include "Physics2D.h"

#include "Logger.h"


Player::Player(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking)
	: Actor(InGameplayState, InLevel, InPosition, IsBlocking)
{
	PreviousPosition = InPosition;
	Health = 100;
}

Player::~Player()
{
}

void Player::Tick(float DeltaTime)
{
	Move(DeltaTime);
}

void Player::MoveForward(float Value)
{
	ForwardAxis = Value;
}

void Player::MoveRight(float Value)
{
	RightAxis = Value;
}


bool Player::OnWallCollision()
{
	return false;
}

void Player::Move(float DeltaTime)
{
	Direction = { glm::cos(CurrentRotation) , glm::sin(CurrentRotation) };
	glm::vec2 RightVector = { glm::cos(CurrentRotation + 3.1415 / 2.0f), glm::sin(CurrentRotation + 3.1415 / 2.0f) };

	/* Movement */
	Velocity += ForwardAxis * Direction;
	Velocity += RightAxis * RightVector;
	Velocity *= MoveSpeed * DeltaTime;

	int CurrentTileX = (int)Position.x;
	int CurrentTileY = (int)Position.y;

	/*
	* The idea is simple:
	* Calculate the next player's position
	* Next X, Next Y
	* And from that position, get the corresponding tile
	* If that tile is a wall
	* Slide along the walls, without moving inside the walls
	*/
	float Radius = 0.1f;
	glm::vec2 NextPosition = Position + Velocity;
	int NextTileX = (NextPosition.x > Position.x) ? (NextPosition.x + Radius) : (NextPosition.x - Radius);

	if (mLevel->GetTileAt(NextTileX, CurrentTileY)->bBlocking == false)
	{
		Position.x = NextPosition.x;
	}
	else
	{
		// Collision
		Position.x = (NextPosition.x > Position.x) ? (glm::round(NextPosition.x) - Radius) : (glm::round(NextPosition.x) + Radius);
	}

	int NextTileY = (NextPosition.y > Position.y) ? (NextPosition.y + Radius) : (NextPosition.y - Radius);


	if (mLevel->GetTileAt(CurrentTileX, NextTileY)->bBlocking == false)
	{
		Position.y = NextPosition.y;
	}
	else
	{
		// Collision
		Position.y = (NextPosition.y > Position.y) ? (glm::round(NextPosition.y) - Radius) : (glm::round(NextPosition.y) + Radius);
	}

	PreviousPosition = Position;


	/* Consume previous inputs */
	Velocity = { 0.0f, 0.0f };
	ForwardAxis = 0.0f;
	RightAxis = 0.0f;
}