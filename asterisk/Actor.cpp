/*
	Asterisk
	Redouane Chabane (July, 2022)
	Actor.cpp - Basic actor class, inherited by all type of "entities" like monsters, player etc;
*/

#include "Actor.h"

Actor::Actor(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking)
	: mGameplayState(InGameplayState), mLevel(InLevel), Position(InPosition), bBlocking(IsBlocking)
{
}

Actor::~Actor()
{
}

void Actor::Tick(float DeltaTime)
{
}

void Actor::MoveForward(float Value)
{
}

void Actor::MoveRight(float Value)
{
}

void Actor::Rotate(float Value)
{
	SetRotation(CurrentRotation + Value);
}

void Actor::SetPosition(const glm::vec2& InPosition)
{
}

void Actor::SetRotation(float Value)
{
	CurrentRotation = std::remainderf(Value, 2 * 3.1415f);
}

void Actor::OnCollision(Actor* OtherActor)
{
}

void Actor::OnWallCollision()
{
}

const glm::vec2& Actor::GetPosition() const
{
	return Position;
}

const glm::vec2& Actor::GetVelocity() const
{
	return Velocity;
}

const float Actor::GetRotation() const
{
	return CurrentRotation;
}
