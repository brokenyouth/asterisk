#pragma once

#include "Game.h"
#include <SDL.h>
#include <memory>

class State
{
protected:
	Game& mGame;

public:
	explicit State(Game& pGame);
	virtual ~State() {}

	virtual void HandleEvent(SDL_Event& pEvent) = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void Render() = 0;

	inline Game& GetGame() const { return mGame; }
};

template <class T>
class StateFactory
{
public:
	static std::unique_ptr<T> Make(Game& pGame)
	{
		return std::make_unique<T>(pGame);
	}
};