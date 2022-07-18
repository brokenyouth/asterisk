/*
	Asterisk
	Redouane Chabane (July, 2022)
	GameplayState.cpp - Actual gameplay code goes here. Input, Update, Render... the rest of the 50% raycasting is done here.
*/

#include "GameplayState.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Player.h"
#include "Level.h"
#include "Utility.h"
#include "Logger.h"

#include <glm/glm.hpp>

#define FOGR 42
#define FOGG 42
#define FOGB 42

GameplayState::GameplayState(Game& pGame) : State(pGame)
{
	mRenderer = mGame.GetRenderer();
	mLevel = std::make_shared<Level>("rc_space.lvl", this, &mGame);
}

void GameplayState::HandleEvent(SDL_Event& pEvent)
{
}

void GameplayState::Update(float DeltaTime)
{
	mLevel->Update(DeltaTime);
	UpdatePlayer(DeltaTime);
}

void GameplayState::Render()
{
	//mRenderer->RenderFill(0x0000000);
	//RenderBackground();
	RenderWorld();
	RenderPlayer();
	//RenderMinimap();
}

void GameplayState::UpdatePlayer(float DeltaTime)
{
	mPlayer = mLevel->GetPlayer();
	if (mPlayer)
	{
		InputManager* Input = InputManager::Get();
		if (Input->KeyHeld(SDL_SCANCODE_W)) // Forward
		{
			mPlayer->MoveForward(1.0f);
		}
		if (Input->KeyHeld(SDL_SCANCODE_S)) // Backward
		{
			mPlayer->MoveForward(-1.0f);
		}
		if (Input->KeyHeld(SDL_SCANCODE_D)) // Right
		{
			mPlayer->MoveRight(-1.0f);
		}
		if (Input->KeyHeld(SDL_SCANCODE_A)) // Left
		{
			mPlayer->MoveRight(1.0f);
		}

		float DeltaMouseX = Input->GetDeltaMouseX();

		mPlayer->Rotate(-DeltaMouseX * 0.005f);

		mPlayer->Tick(DeltaTime);
	}
}

void GameplayState::RenderMinimap()
{

	for (auto& _Tile : mLevel->GetTiles())
	{
		if (_Tile.bBlocking)
			mRenderer->RenderRect(_Tile.TileCoordinate.x * MiniMapScale, _Tile.TileCoordinate.y * MiniMapScale, 12, 0x000000FF);
	}

	Player* _Player = mLevel->GetPlayer();
	if (_Player)
	{
		RenderMinimapPointOfView();
	}

}

void GameplayState::RenderBackground()
{
	const int HalfScreenHeight = (int)(mRenderer->GetFrameHeight() / 2);
	Texture* NightSkyTexture = ResourceManager::Get()->GetTexture("night_sky");
	mRenderer->RenderTexture(NightSkyTexture, 0, 0);
	mRenderer->RenderRect(0, HalfScreenHeight + 1, mRenderer->GetFrameWidth(), HalfScreenHeight, 0xFF262016);
}

void GameplayState::RenderWorld()
{
	Texture* NightSkyTexture = ResourceManager::Get()->GetTexture("night_sky");
	Texture* MissingTexture = ResourceManager::Get()->GetTexture("missing");
	uint32_t FloorColor = 0xFF262016;
	const Player* _Player = mLevel->GetPlayer();
	const int ScreenWidth = mRenderer->GetFrameWidth();
	const int ScreenHeight = mRenderer->GetFrameHeight();

	float HalfVerticalFov = (static_cast<float>(ScreenHeight) / static_cast<float>(ScreenWidth)) * (mRenderer->GetFOV() / 2.0f);
	float TanHalfVerticalFov = glm::tan(HalfVerticalFov);
	// sample screen columns
	for (int x = 0; x < ScreenWidth; x++)
	{
		const float ViewPlaneDistance = (ScreenWidth / (2 * glm::tan(mRenderer->GetFOV() / 2.0f)));
		
		// Calculate the ray angle taking into account the view plane distance (e.g the distance from which rays are cast)
		const float ScreenAngle = glm::atan((x - ScreenWidth / 2) / ViewPlaneDistance);
		const float RayAngle = _Player->GetRotation() + ScreenAngle;
		const glm::vec2 RayDirection = { glm::cos(RayAngle), glm::sin(RayAngle) };

		// Get Raycast information
		const RaycastHitInfo RayHitInfo = mLevel->Raycast(_Player->GetPosition(), RayDirection, 2000.0f);
		const float RealHitDistance = glm::length(_Player->GetPosition() - RayHitInfo.HitPosiiton);
		const float HitDistance = RealHitDistance * /* fisheye fix */ glm::cos(ScreenAngle) ;
	
		const int ColumnHeight = (int)(ScreenHeight / ( 2 * HitDistance * TanHalfVerticalFov));

		if (ColumnHeight)
		{
			int ColumnStartY = (ScreenHeight / 2) - (ColumnHeight / 2);
			int ColumnEndY = ColumnStartY + ColumnHeight;

			ColumnStartY = ColumnStartY < 0 ? 0 : ColumnStartY;
			ColumnEndY = ColumnEndY > (ScreenHeight - 1) ? ScreenHeight - 1 : ColumnEndY;

			int ColumnScreenHeightDelta = ColumnHeight - ScreenHeight;

			float FogReduction = 1 - glm::min(1.f, RealHitDistance * 0.075f);
			
			// Draw ceiling
			for (int y = 0; y < ColumnStartY; y++)
			{
				mRenderer->RenderPixel(x, y, NightSkyTexture->GetPixelColor(x, y));
			}

			// Draw Column (wall)
			for (int y = ColumnStartY; y < ColumnEndY; y++)
			{
				int d = y * 2 + ColumnScreenHeightDelta;
				Texture* HitTexture = RayHitInfo.HitTile->TileTexture;
				uint32_t PixelColor = 0;
				if (!HitTexture)
				{
					HitTexture = MissingTexture;
				}
				int TextureX = (int)(HitTexture->GetWidth() * RayHitInfo.TexturePlanePosition);
				int TextureY = d * HitTexture->GetHeight() / (2 * ColumnHeight);

				PixelColor = HitTexture->GetPixelColor(TextureX, TextureY);
				PixelColor = Util::Color::ApplyFogToPixel(PixelColor, FogReduction);

				mRenderer->RenderPixel(x, y, PixelColor);
			}

			// Draw floor
			// Well... this actually created a pretty cool floor see-through effect
			for (int y = ColumnEndY; y < ScreenHeight; y++)
			{
				uint32_t FloorColorWithFog = Util::Color::ApplyFogToPixel(FloorColor, FogReduction);
				mRenderer->RenderPixel(x, y, FloorColorWithFog);
			}
		}
	}
}

void GameplayState::RenderPlayer()
{
	/*
	* Render weapon
	*/
	Texture* CurrWeaponTexture = mPlayer->Weapon->GetTexture();
	const glm::vec2& DeltaWeaponPosition = mPlayer->Weapon->GetPosition();
	glm::vec2 WeaponDrawPosition = { (mRenderer->GetFrameWidth() / 2 ) - (CurrWeaponTexture->GetWidth() / 2),
		mRenderer->GetFrameHeight() - (CurrWeaponTexture->GetHeight() )
	};

	WeaponDrawPosition -= DeltaWeaponPosition;
	mRenderer->RenderTexture(CurrWeaponTexture, WeaponDrawPosition.x, WeaponDrawPosition.y);
	/**************************/
}

void GameplayState::RenderMinimapPointOfView()
{
	const Player* _Player = mLevel->GetPlayer();
	const glm::vec2& Position = _Player->GetPosition();
	const int LineLen = 100;
	mRenderer->RenderRect(Position.x * MiniMapScale, Position.y * MiniMapScale, 8, 0x00FF00FF);

	const int LineStartX = (Position.x + 0.5) * MiniMapScale;
	const int LineStartY = (Position.y + 0.5) * MiniMapScale;
	const int LineEndX = LineStartX + glm::cos(_Player->GetRotation()) * 400;
	const int LineEndY = LineStartY + glm::sin(_Player->GetRotation()) * 400;
	mRenderer->RenderLine(LineStartX, LineStartY, LineEndX, LineEndY, 0xFF00FFFF);
}
