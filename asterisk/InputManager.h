#pragma once

#include <SDL.h>
#include <unordered_map>
#include <vector>


class InputManager
{
private:
	static InputManager* sInstance;
	int DeltaMouseX;
	int DeltaMouseY;

private:
	std::unordered_map<SDL_Scancode, int> mHeldKeyMap;
	std::unordered_map<SDL_Scancode, int> mPressedKeyMap;
	std::unordered_map<SDL_Scancode, int> mReleasedKeyMap;

	InputManager();
	~InputManager();

public:
	static InputManager* Get();
	void Release();

public:
	void BeginNewFrame();
	void OnKeyDown(const SDL_Event& event);
	void OnKeyUp(const SDL_Event& event);
	void OnMouseMotion(const SDL_Event& event);

	bool KeyDown(SDL_Scancode Key);
	bool KeyUp(SDL_Scancode Key);
	bool KeyHeld(SDL_Scancode Key);

	int GetDeltaMouseX() const;
	int GetDeltaMouseY() const;
};