#pragma once

#include "State.h"

class Level;
class Player;
class Renderer;

class GameplayState : public State
{
private:
    std::shared_ptr<Renderer> mRenderer { nullptr };
    std::shared_ptr<Level> mLevel { nullptr };
    Player* mPlayer = nullptr;

    int MiniMapScale = 12;

public:
    explicit GameplayState(Game& pGame);

    virtual void HandleEvent(SDL_Event& pEvent) override;
    virtual void Update(float pDeltaTime) override;
    virtual void Render() override;

private:
    void UpdatePlayer(float DeltaTime);
    void RenderMinimap();
    void RenderBackground();
    void RenderWorld();
    void RenderPlayer();
    void RenderMinimapPointOfView();
};