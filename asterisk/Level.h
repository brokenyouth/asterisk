#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Player.h"

class Game;
class Actor;
class Texture;
class Tile;
class GameplayState;

struct RaycastHitInfo
{
    glm::vec2 HitPosiiton;
    Tile* HitTile; // Pointer to the hit tile
    bool bHitWall; // Is this a wall?
    float TexturePlanePosition; // 
};

class Tile
{
public:
    int Size;
    glm::vec2 TileCoordinate = {0, 0}; // Tile Coordinates
    Texture* TileTexture = nullptr; // Pointer to Tile Texture object
    bool bBlocking = false; // Is this Tile a blocking tile or not? e.g wall, door -- or an empty tile
    bool bIsDoor = false; // Is this tile a door?
public:
    Tile() = default;
    explicit Tile(int InSize, Texture* InTileTexture, glm::vec2 InTileCoordinate)
    {
        Size = InSize;
        TileTexture = InTileTexture;
        TileCoordinate = InTileCoordinate;
    }
};

class Level
{
private:
    std::unordered_map<uint32_t, Texture*> mTextureLookup; // Find texture by assigned index
    std::vector<Tile> mTiles;
    Game* mGame = nullptr;
    GameplayState* mGameplayState = nullptr;
    std::shared_ptr<Player> mPlayer = nullptr;

private:
    void LoadLevel(const std::string& LevelName);
    void HandleRowData(int RowNumber, const std::string& RowData);

public:
    int TileSize;
    int Width;
    int Height;

public:

    Level(const std::string& LevelName, GameplayState* pGameplayState, Game* pGame);
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    ~Level();

public:
    void Update(float DeltaTime);
    RaycastHitInfo Raycast(const glm::vec2& RayPosition, const glm::vec2& RayDirection, float MaxDistance = 1000.f);
    
    Game* GetGame() const;
    Player* GetPlayer() const;

    inline int GetLevelTileSize() const {
        return TileSize;
    }
    const std::vector<Tile>& GetTiles() const;
    Tile* GetTileAt(int x, int y);
};
