#pragma once

#include "InputManager.h"

#include <glm/glm.hpp>

class GameplayState;
class Level;
class Texture;

class Actor
{
protected:
	GameplayState* mGameplayState = nullptr;
	Level* mLevel = nullptr;
	Texture* mTexture = nullptr;

	glm::vec2 Position = { 0.f, 0.f };
	glm::vec2 Direction = {0.f, 0.f};
	glm::vec2 Velocity = { 0.f, 0.f };
	float ForwardSpeed = 0.f;
	float SideSpeed = 0.f;
	float CurrentRotation = 0.f;
	bool bBlocking = true;

public:
	explicit Actor(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking);
	virtual ~Actor();

	/*
	* Actor update function
	*/
	virtual void Tick(float DeltaTime);

	/*
	* Movement is axis based.
	* Value is a float:
	* 1 for forward, -1 for backward
	* 1 for right, -1 for left
	*/
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void Rotate(float Value);

	virtual void SetPosition(const glm::vec2& InPosition);
	virtual void SetRotation(float Value);

	virtual void OnCollision(Actor* OtherActor);
	virtual void OnWallCollision();

	virtual Texture* GetTexture() const { return mTexture; }


	virtual const glm::vec2& GetPosition() const;
	virtual const glm::vec2& GetVelocity() const;
	virtual const float GetRotation() const;


};