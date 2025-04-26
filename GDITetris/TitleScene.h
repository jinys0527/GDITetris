#pragma once

#include "Scene.h"

class GameObjectBase;
class Tetris;

class TitleScene : public Scene
{
public:
    TitleScene() = default;
    ~TitleScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    void OnKeyDown(int key) override;
private:

    GameObjectBase* m_pBackground = nullptr;
    //GameObjectBase* m_pStartButton = nullptr;

    Tetris* m_pGame = nullptr;

    WCHAR m_szTitle[64] = L"Title Scene";

    RECT m_rect = { 0, 0, 0, 0 };

    bool isTrigger = false;
};

