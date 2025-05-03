#pragma once
#include "Scene.h"

class BackGround;
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

    void OnKeyDown(int key) {}
    void OnKeyUp(int key) {}
    void OnClicked(int x, int y) {}

private:
    Tetris* m_pGame = nullptr;
    BackGround* m_pBackground = nullptr;
};

