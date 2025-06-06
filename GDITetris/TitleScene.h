#pragma once

#include "Scene.h"

class GameObjectBase;
class Background;
class Tetris;
class SoundManager;

class TitleScene : public Scene
{
public:
    TitleScene() : Scene() {};
    ~TitleScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    void OnClicked(int x, int y) override;
private:

    Background* m_pBackground = nullptr;
    Background* m_pKey = nullptr;
    Background* m_pButton = nullptr;

    Tetris* m_pGame = nullptr;

    WCHAR m_szTitle[64] = L"Title Scene";

    RECT m_rect = { 0, 0, 0, 0 };

    bool m_isClickedStart = false;
    bool m_isClickedMethod = false;
};

