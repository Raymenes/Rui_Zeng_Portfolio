#include "InputManager.h"
#include <SDL/SDL.h>


void InputManager::HandleKeyPressed(int key)
{
    auto ptr = mKeyToActionMap.find(key);
    if (ptr != mKeyToActionMap.end()) {
        if (ptr->second->mPressedDelegate != nullptr) {
            ptr->second->mPressedDelegate->Execute();
        }
    }
    
    auto ptr2 = mKeyToAxisMap.find(key);
    if (ptr2 != mKeyToAxisMap.end()) {
        if (ptr2->second->mDelegate != nullptr) {
            if (key == ptr2->second->mPositiveKey) {
                ptr2->second->mValue += 1.0f;
                ptr2->second->mDelegate->Execute(ptr2->second->mValue);
            }
            else if (key == ptr2->second->mNegativeKey)
            {
                ptr2->second->mValue -= 1.0f;
                ptr2->second->mDelegate->Execute(ptr2->second->mValue);
            }
        }
    }
}

void InputManager::HandleKeyReleased(int key)
{
    auto ptr = mKeyToActionMap.find(key);
    if (ptr != mKeyToActionMap.end()) {
        if (ptr->second->mReleasedDelegate != nullptr) {
            ptr->second->mReleasedDelegate->Execute();
        }
    }
    
    auto ptr2 = mKeyToAxisMap.find(key);
    if (ptr2 != mKeyToAxisMap.end()) {
        if (ptr2->second->mDelegate != nullptr) {
            if (key == ptr2->second->mPositiveKey) {
                ptr2->second->mValue -= 1.0f;
                ptr2->second->mDelegate->Execute(ptr2->second->mValue);
            }
            else if (key == ptr2->second->mNegativeKey)
            {
                ptr2->second->mValue += 1.0f;
                ptr2->second->mDelegate->Execute(ptr2->second->mValue);
            }
        }
    }
}

void InputManager::AddActionMapping(const std::string& name, int key)
{
	ActionMappingPtr ptr = std::make_shared<ActionMapping>();
    ptr->mName = name;
    ptr->mKey = key;
    mNameToActionMap.emplace(name, ptr);
    mKeyToActionMap.emplace(key, ptr);
}

void InputManager::AddAxisMapping(const std::string& name, int positiveKey, int negativeKey)
{
	AxisMappingPtr ptr = std::make_shared<AxisMapping>();
    
    ptr->mName = name;
    ptr->mPositiveKey = positiveKey;
    ptr->mNegativeKey = negativeKey;
    ptr->mValue = 0.0f;
    
    mNameToAxisMap.emplace(name, ptr);
    mKeyToAxisMap.emplace(positiveKey, ptr);
    mKeyToAxisMap.emplace(negativeKey, ptr);
}

void InputManager::BindActionInternal(const std::string& name, InputEvent event, ActionDelegatePtr delegate)
{
    auto iter = mNameToActionMap.find(name);
    if (iter != mNameToActionMap.end())
    {
        if (event == IE_Pressed) {
            iter->second->mPressedDelegate = delegate;
        }
        else if (event == IE_Released)
            iter->second->mReleasedDelegate = delegate;
    }
}

void InputManager::BindAxisInternal(const std::string& name, AxisDelegatePtr delegate)
{
    auto iter = mNameToAxisMap.find(name);
    if (iter != mNameToAxisMap.end()){
        iter->second->mDelegate = delegate;
    }
}

Vector2 InputManager::GetMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return Vector2(x, y);
}
