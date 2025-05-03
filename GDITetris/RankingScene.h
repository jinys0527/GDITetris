#pragma once
#include "Scene.h"
#include "RankingManager.h"

class Background;
class Tetris;

class RankingScene : public Scene
{
public:
    RankingScene() {}
    ~RankingScene() {}
    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override;
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    void OnKeyDown(int key) override;
private:
    Tetris* m_pGame = nullptr;
    Background* m_pBackground = nullptr;
    RankingManager m_rakingManger;
};

