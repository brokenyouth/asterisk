#pragma once

#include "Actor.h"


class IWeapon
{
protected:
	Actor* Owner = nullptr;
	Texture* CurrentTexture = nullptr;
	glm::vec2 Position;
protected:
	virtual bool CanDeploy() = 0;

public:
	IWeapon(Actor* WeaponOwner, Texture* WeaponTexture) :
		Owner(WeaponOwner), CurrentTexture(WeaponTexture)
	{

	}
	virtual void Update(float DeltaTime) = 0;
	virtual void Reload() = 0;
	virtual void Shoot() = 0;
	virtual void Deploy() = 0;
	virtual Texture* GetTexture() const {
		return CurrentTexture;
	}
	virtual const glm::vec2& GetPosition() const {
		return Position;
	}
};

class WeaponRifle : public IWeapon
{
private:
	/*
	* Used for weapon bobbing
	*/
	float Speed;
	float Time; 
	float Amplitude;
	/***************/
	int DefaultAmmo;
	int DefaultClipSize;
public:
	int Ammo;
	int ClipSize;

protected:
	virtual bool CanDeploy();

public:
	WeaponRifle(Actor* WeaponOwner, Texture* WeaponTexture);
	virtual ~WeaponRifle();
	virtual void Update(float DeltaTime) override;
	virtual void Reload() override;
	virtual void Shoot() override;
	virtual void Deploy() override;
};

class Player : public Actor
{
protected:
	float ForwardAxis = 0.0f;
	float RightAxis = 0.0f;
	float FieldOfView = 78.f;

public:
	IWeapon* Weapon = nullptr;
	glm::vec2 PreviousPosition;
	float MoveSpeed = 8.f;
	int Health;


private:
	void Move(float Dt);

public:
	Player(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking);
	~Player();

	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

	virtual bool OnWallCollision() override;



};