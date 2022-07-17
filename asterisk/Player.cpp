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
#include "InputManager.h"
#include "ResourceManager.h"
#include "Utility.h"

#include "Logger.h"

Player::Player(GameplayState* InGameplayState, Level* InLevel, const glm::vec2& InPosition, bool IsBlocking)
	: Actor(InGameplayState, InLevel, InPosition, IsBlocking)
{
	Texture* WeaponTexture = ResourceManager::Get()->GetTexture("rifle_idle");
	Weapon = new WeaponRifle(this, WeaponTexture);
	PreviousPosition = InPosition;
	Health = 100;
}

Player::~Player()
{
	delete Weapon;
	Weapon = nullptr;
}

void Player::Tick(float DeltaTime)
{
	Move(DeltaTime);
	Weapon->Update(DeltaTime);
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
	//Velocity = { 0.0f, 0.0f };
	ForwardAxis = 0.0f;
	RightAxis = 0.0f;
}

bool WeaponRifle::CanDeploy()
{
	return false;
}

WeaponRifle::WeaponRifle(Actor* WeaponOwner, Texture* WeaponTexture) : IWeapon(WeaponOwner, WeaponTexture)
{
	DefaultAmmo = 12;
	DefaultClipSize = 90;
	Ammo = DefaultAmmo;
	ClipSize = DefaultClipSize;
	Position = { 0.f, 0.f };
}

WeaponRifle::~WeaponRifle()
{

}

void WeaponRifle::Update(float DeltaTime)
{
	InputManager* Input = InputManager::Get();
	Time += DeltaTime;
	if (Input->KeyHeld(SDL_SCANCODE_W) || Input->KeyHeld(SDL_SCANCODE_S) || Input->KeyHeld(SDL_SCANCODE_D) || Input->KeyHeld(SDL_SCANCODE_A))
	{
		// TODO: Avoid hardcode
		Multiplier = 12.f;
		Amplitude = 100.0f;
	}
	else
	{
		Time = 0.f;
		Multiplier = 4.f;
		Amplitude = 50.0f;
		Position = Util::Vector2Lerp(Position, { 0.f, 0.f }, DeltaTime * 20.f);
	}

	float Frequency = static_cast<float>(Multiplier);
	float MovementX = glm::sin(Time * Frequency) * Amplitude;
	float MovementY = glm::sin(Time * Frequency) * Amplitude;

	Position.x += MovementX * DeltaTime;
	Position.y += MovementY * 0.075f * DeltaTime;

}

void WeaponRifle::Reload()
{
	if (ClipSize > 0 && Ammo < DefaultAmmo)
	{
		int AmmoDifference = DefaultAmmo - Ammo;
		int AmmoToInsert = glm::min(AmmoDifference, ClipSize);
		ClipSize = glm::max(ClipSize - AmmoToInsert, 0);
		Ammo += AmmoToInsert;
	}
}

void WeaponRifle::Shoot()
{
	if (Ammo > 0)
	{
		Ammo--;
	}

	if (Ammo == 0)
	{
		Reload();
	}
}

void WeaponRifle::Deploy()
{
	if (Ammo > 0 && ClipSize > 0)
	{
		// Do something...
	}
}
