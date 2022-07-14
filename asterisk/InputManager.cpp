#include "InputManager.h"
#include "Logger.h"

InputManager* InputManager::sInstance = nullptr;

InputManager::InputManager()
{

}

InputManager::~InputManager()
{
}

InputManager* InputManager::Get()
{
    if (sInstance == nullptr)
    {
        sInstance = new InputManager();
        return sInstance;
    }
    return sInstance;
}

void InputManager::Release()
{
    if (sInstance)
    {
        delete sInstance;
        sInstance = nullptr;
        #ifdef _DEBUG
        LOG_CORE_INFO("InputManager is released.");
        #endif
    }
}

void InputManager::BeginNewFrame()
{
    //mHeldKeyMap.clear();
    mPressedKeyMap.clear();
    mReleasedKeyMap.clear();

    SDL_GetRelativeMouseState(&DeltaMouseX, &DeltaMouseY);

}

void InputManager::OnKeyDown(const SDL_Event& event)
{
    SDL_Scancode Key = event.key.keysym.scancode;
    mHeldKeyMap[Key] = true;
    mPressedKeyMap[Key] = true;
}

void InputManager::OnKeyUp(const SDL_Event& event)
{
    SDL_Scancode Key = event.key.keysym.scancode;
    mHeldKeyMap[Key] = false;
    mReleasedKeyMap[Key] = true;
}

void InputManager::OnMouseMotion(const SDL_Event& event)
{
    DeltaMouseX = event.motion.xrel;
    DeltaMouseY = event.motion.yrel;
}

bool InputManager::KeyDown(SDL_Scancode Key)
{
    return mPressedKeyMap[Key];
}

bool InputManager::KeyUp(SDL_Scancode Key)
{
    return mReleasedKeyMap[Key];
}

bool InputManager::KeyHeld(SDL_Scancode Key)
{
    return mHeldKeyMap[Key];
}

int InputManager::GetDeltaMouseX() const
{
    return DeltaMouseX;
}

int InputManager::GetDeltaMouseY() const
{
    return DeltaMouseY;
}
