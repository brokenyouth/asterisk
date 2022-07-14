#include "Level.h"

#include "Game.h"
#include "GameplayState.h"
#include "Player.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Utility.h"

#include <exception>
#include <format>
#include <fstream>

Level::Level(const std::string& LevelName, GameplayState* pGameplayState, Game* pGame) :
	mGame(pGame), mGameplayState(pGameplayState)
{
	mTiles = {};
	LoadLevel(LevelName);
}

Level::~Level()
{

}

void Level::Update(float DeltaTime)
{
}

RaycastHitInfo Level::Raycast(const glm::vec2& RayPosition, const glm::vec2& RayDirection, float MaxDistance)
{
	RaycastHitInfo OutHitInfo {};

	glm::vec2 TilePosition = { (int)RayPosition.x, (int)RayPosition.y };
	glm::vec2 WorldPosition = RayPosition;

	// Pre-calculations to perform DDA
	const float DeltaDistX = std::abs(1 / RayDirection.x);
	const float DeltaDistY = std::abs(1 / RayDirection.y);

	bool bHitWall = false;
	bool bHitHorizontalSide = false;

	// In case unexpected behaviour occurs, exit loop after some time
	int SecurityCheckCount = 0;
	// Perform DDA
	while (!bHitWall)
	{
		glm::vec2 SideDistance{ 0, 0 };
		glm::vec2 StepDirection{ 0, 0 };

		if (RayDirection.x < 0)
		{
			StepDirection.x = -1;
			SideDistance.x = (WorldPosition.x - TilePosition.x) * DeltaDistX;
		}
		else
		{
			StepDirection.x = 1;
			SideDistance.x = (TilePosition.x + 1.0 - WorldPosition.x) * DeltaDistX;
		}
		if (RayDirection.y < 0)
		{
			StepDirection.y = -1;
			SideDistance.y = (WorldPosition.y - TilePosition.y) * DeltaDistY;
		}
		else
		{
			StepDirection.y = 1;
			SideDistance.y = (TilePosition.y + 1.0 - WorldPosition.y) * DeltaDistY;
		}

		// Follow the smaller side distance, you do not want to miss any tile
		if (SideDistance.x < SideDistance.y)
		{
			WorldPosition += RayDirection * SideDistance.x;
			TilePosition.x += StepDirection.x;
			bHitHorizontalSide = true;
		}
		else
		{
			WorldPosition += RayDirection * SideDistance.y;
			TilePosition.y += StepDirection.y;
			bHitHorizontalSide = false;
		}

		Tile* CurrentTile = GetTileAt(TilePosition.x, TilePosition.y);
		if (CurrentTile && CurrentTile->bBlocking) // Is a wall
		{
			bHitWall = true;
		}

		if (glm::distance(mPlayer->GetPosition(), WorldPosition) > MaxDistance)
		{
			break;
		}
	}

	Tile* HitTile = GetTileAt(TilePosition.x, TilePosition.y);
	if (bHitWall) OutHitInfo.bHitWall = true;
	OutHitInfo.HitPosiiton = WorldPosition;
	OutHitInfo.HitTile = HitTile;

	if (bHitHorizontalSide)
	{
		OutHitInfo.TexturePlanePosition = std::abs(WorldPosition.y - TilePosition.y);
		if (WorldPosition.x > TilePosition.x)
		{
			OutHitInfo.TexturePlanePosition = 1.0f - OutHitInfo.TexturePlanePosition;
		}
	}
	else
	{
		OutHitInfo.TexturePlanePosition = 1.0f - std::abs(WorldPosition.x - TilePosition.x);
		if (WorldPosition.y > TilePosition.y)
		{
			OutHitInfo.TexturePlanePosition = 1.0f - OutHitInfo.TexturePlanePosition;
		}
	}

	return OutHitInfo;
}

Game* Level::GetGame() const
{
	return mGame;
}

Player* Level::GetPlayer() const
{
	return mPlayer.get();
}

const std::vector<Tile>& Level::GetTiles() const
{
	return mTiles;
}

Tile* Level::GetTileAt(int x, int y)
{
	if (x >= 0 && y >= 0 && x < Width && y < Height)
		return &mTiles[y * Width + x];
	return nullptr;
}

void Level::LoadLevel(const std::string& LevelName)
{
#ifdef _DEBUG
	LOG_INFO("Loading Level ... [ {} ]", LevelName);
#endif
	// TODO: Hardcoded
	const std::string LevelPath = "data/levels/";
	std::ifstream FileStream(LevelPath + LevelName.c_str());
	std::string Line;
	int RowNumber = 0;

	while (std::getline(FileStream, Line))
	{
		std::istringstream Iss(Line);

		if (Util::StringStartsWith(Line, "tilesize"))
		{
			std::string Temp, TileWidth, TileHeight;
			if (!(Iss >> Temp >> TileWidth >> TileHeight))
			{
				const std::string Message = std::format("Error loading level : {}\nIncorrect tile dimensions", LevelName);
				throw std::runtime_error(Message);
			}
			// SetTile Dimensions
		}
		else if (Util::StringStartsWith(Line, "gridsize"))
		{
			std::string Temp, GridWidthStr, GridHeightStr;
			if (!(Iss >> Temp >> GridWidthStr >> GridHeightStr))
			{
				const std::string Message = std::format("Error loading level : {}\nIncorrect grid dimensions", LevelName);
				throw std::runtime_error(Message);
			}
			// SetGrid Dimensions
			Width = std::stoi(GridWidthStr);
			Height = std::stoi(GridHeightStr);
		}
		else if (Util::StringStartsWith(Line, "player"))
		{
			std::string Temp, PlayerStartX, PlayerStartY;
			if (!(Iss >> Temp >> PlayerStartX >> PlayerStartY))
			{
				const std::string Message = std::format("Error loading level : {}\nIncorrect player start", LevelName);
				throw std::runtime_error(Message);
			}
			// SetPlayer Start Position
			const int PlayerX = std::stoi(PlayerStartX);
			const int PlayerY = std::stoi(PlayerStartY);
			glm::vec2 PlayerTilePosition = glm::vec2(PlayerX, PlayerY);
			mPlayer = std::make_unique<Player>(mGameplayState, this, PlayerTilePosition, true);
		}

		else if (Util::StringStartsWith(Line, "enemy"))
		{
			std::string Temp, EnemyStartX, EnemyStartY;
			if (!(Iss >> Temp >> EnemyStartX >> EnemyStartY))
			{
				const std::string Message = std::format("Error loading level : {}\nIncorrect enemy start", LevelName);
				throw std::runtime_error(Message);
			}
			// SetEnemy Start Position
		}

		else if (Util::StringStartsWith(Line, "grid"))
		{
			std::string GridStr, RowDataStr;
			if (!(Iss >> GridStr >> RowDataStr))
			{
				const std::string Message = std::format("Error loading level : {}\nIncorrect grid row", LevelName);
				throw std::runtime_error(Message);
			}
			HandleRowData(RowNumber, RowDataStr);
			RowNumber++;
		}
		else
		{
			if (std::isalnum(Line[0]))
			{
				std::string Idx, TextureName;
				if (!(Iss >> Idx >> TextureName))
				{
					const std::string Message = std::format("Error loading level : {}\nIncorrect texture mapping", LevelName);
					throw std::runtime_error(Message);
				}
				mTextureLookup[std::stoi(Idx)] = ResourceManager::Get()->GetTexture(TextureName);
			}
		}
	}
#ifdef _DEBUG
	LOG_INFO("mTiles.size() ... [ {} ]", mTiles.size());
#endif
}

void Level::HandleRowData(int RowNumber, const std::string& RowData)
{
	for (auto i = 0; i < RowData.size(); ++i)
	{
		Tile NewTile;
		glm::vec2 TileCoords = { i , RowNumber };
		if (std::isalnum(RowData[i])) // is a wall
		{
			int TextureIndex = RowData[i] - '0';
			NewTile = Tile(mTextureLookup[TextureIndex], TileCoords);
			if (NewTile.TileTexture)
			{
				//LOG_INFO("Texture tile ptr => {}", NewTile.TileTexture->GetPath());
			}
			NewTile.bBlocking = true;
			LOG_INFO("Found blocking tile");
		}
		else
		{
			NewTile = Tile(ResourceManager::Get()->GetTexture("missing"), TileCoords);
			NewTile.bBlocking = false;
		}
		
		mTiles.push_back(std::move(NewTile));
	}
}
