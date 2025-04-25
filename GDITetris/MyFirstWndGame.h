#pragma once
#include "NzWndBase.h"
#include "Utillity.h"
#include "Scene.h"

// [CHECK] #1  ���� ������ ����Ͽ� ��������� �������� ����.
class GameTimer;
class GameObjectBase;
class GameObject;

namespace renderHelp
{
    class BitmapInfo;
    class SpriteSheet;
}

class MyFirstWndGame : public NzWndBase
{
public:
    MyFirstWndGame() = default;
    ~MyFirstWndGame() override = default;

    bool Initialize();
    void Run();
    void Finalize();

private:
    void Update();
    void Render();

    void OnResize(int width, int height) override;
    void OnClose() override;

    void OnKeyDown(int key);

    void FixedUpdate();
    void LogicUpdate();

private:
    HDC m_hFrontDC = nullptr;
    HDC m_hBackDC = nullptr;
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hDefaultBitmap = nullptr;

    // [CHECK] #8. ���� Ÿ�̸Ӹ� ����Ͽ� �������� �����ϴ� ����.
    GameTimer* m_pGameTimer = nullptr;
    float m_fDeltaTime = 0.0f;
    float m_fFrameCount = 0.0f;

    // [CHECK] #8. ���� ������Ʈ�� �����ϴ� �����̳�.
    // GameObjectBase** m_GameObjectPtrTable = nullptr; Scene����

    struct MOUSE_POS
    {
        int x = 0;
        int y = 0;

        bool operator!=(const MOUSE_POS& other) const
        {
            return (x != other.x || y != other.y);
        }
    };

    MOUSE_POS m_MousePos = { 0, 0 };
    MOUSE_POS m_MousePosPrev = { 0, 0 };

#pragma region resource
    using BitmapInfo = renderHelp::BitmapInfo;
    using SpriteSheet = renderHelp::SpriteSheet;
    BitmapInfo* m_pBackgroundBitmapInfo = nullptr;
    SpriteSheet* m_pBricksSpriteSheet = nullptr;

#pragma endregion
    int m_eCurrentScene = SCENE_TITLE;
    Scene* m_pScenes[SceneType::SCENE_MAX] = { nullptr, nullptr, nullptr };

public:
    BitmapInfo* GetBackgroundBitmapInfo() const;
    SpriteSheet* GetBricksSpriteSheet() const { return m_pBricksSpriteSheet; }
    void ChangeScene(SceneType eSceneType);
};