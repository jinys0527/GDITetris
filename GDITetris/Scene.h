#pragma once
#include "INC_Windows.h"

enum SceneType
{
    SCENE_TITLE = 0,
    SCENE_PLAY,
    SCENE_RANKING,
    SCENE_MAX
};

class GameObjectBase;
class NzWndBase;
class SoundManager;

class Scene
{
public:
    Scene() : m_pSoundManager(nullptr) {};
    virtual ~Scene() = default;
    virtual void Initialize(NzWndBase* pWnd) = 0;
    virtual void Finalize() = 0;

    virtual void Enter() = 0;
    virtual void Leave() = 0;

    virtual void FixedUpdate() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(HDC hDC) = 0;

    virtual void OnKeyDown(int key) {}
    virtual void OnKeyUp(int key) {}
    virtual void OnClicked(int x, int y) {}

protected:
    GameObjectBase** m_GameObjectPtrTable = nullptr;
    SoundManager* m_pSoundManager;
private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
};


