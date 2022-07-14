#pragma once

#include "Actor.h"

class Player : public Actor
{
protected:
	float ForwardAxis = 0.0f;
	float RightAxis = 0.0f;
	float MoveSpeed = 8.f;
	int Health;
	float FieldOfView = 78.f;

public:
	Player(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking);
	~Player();

	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

};