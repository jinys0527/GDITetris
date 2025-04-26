#pragma once
#include "Scene.h"

class Tetris;

class EndingScene : public Scene
{
public:
    EndingScene() = default;
    ~EndingScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    void OnKeyDown(int key) override;
private:
    Tetris* m_pGame = nullptr;

    WCHAR m_szTitle[64] = L"Ending Scene";

    RECT m_rect = { 0, 0, 0, 0 };

    bool isTrigger = false;
};

