#include "Tetris.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include <iostream>
#include <assert.h>
#include "RenderHelp.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "EndingScene.h"

using namespace learning;
using namespace renderHelp;

//constexpr int MAX_GAME_OBJECT_COUNT = 1000;

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)

bool Tetris::Initialize()
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();

    const wchar_t* className = L"Tetris";
    const wchar_t* windowName = L"GDITetris";

    if (false == __super::Create(className, windowName, 1024, 720))
    {
        return false;
    }

    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);
    
#pragma region resource
    m_pBackgroundBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/title.png");
    m_pBricksSpriteSheet = new SpriteSheet();
    if (!m_pBricksSpriteSheet->LoadFromJson(L"../Resource/bricks.json") ||
        !m_pBricksSpriteSheet->LoadSprite(L"../Resource/bricks.png"))
    {
        std::cout << "SpriteSheet Load Failed!" << std::endl;
        return false;
    }

    if (m_pBackgroundBitmapInfo == nullptr)
    { 
        std::cout << "Bitmap Load Failed!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Loaded" << std::endl;
    }

#pragma endregion
    
    m_pScenes[SceneType::SCENE_TITLE] = new TitleScene();
    m_pScenes[SceneType::SCENE_TITLE]->Initialize(this);
    m_pScenes[SceneType::SCENE_PLAY] = new PlayScene();
    m_pScenes[SceneType::SCENE_PLAY]->Initialize(this);
    m_pScenes[SceneType::SCENE_ENDING] = new EndingScene();
    m_pScenes[SceneType::SCENE_ENDING]->Initialize(this);

    return true;

}

void Tetris::Run()
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN)
            {
                m_pScenes[m_eCurrentScene]->OnKeyDown(msg.wParam);
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void Tetris::Finalize()
{
    delete m_pGameTimer;
    m_pGameTimer = nullptr;

    if (m_pBricksSpriteSheet)
    {
        delete m_pBricksSpriteSheet;
        m_pBricksSpriteSheet = nullptr;
    }

    for (int i = 0; i < SceneType::SCENE_MAX; i++)
    {
        if (m_pScenes[i])
        {
            m_pScenes[i]->Finalize();
            delete m_pScenes[i];
            m_pScenes[i] = nullptr;
        }
    }

    __super::Destroy();
}

void Tetris::FixedUpdate()
{
    m_pScenes[m_eCurrentScene]->FixedUpdate();
}

void Tetris::LogicUpdate()
{

    m_pScenes[m_eCurrentScene]->Update(m_fDeltaTime);
}

BitmapInfo* Tetris::GetBackgroundBitmapInfo() const
{
    return m_pBackgroundBitmapInfo;
}

void Tetris::ChangeScene(SceneType eSceneType)
{
    if (m_eCurrentScene != eSceneType)
    {
        m_pScenes[m_eCurrentScene]->Leave();
        m_eCurrentScene = eSceneType;
        m_pScenes[m_eCurrentScene]->Enter();
    }
}

void Tetris::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate();

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;

    while (m_fFrameCount >= 200.0f)
    {
        FixedUpdate();
        m_fFrameCount -= 200.0f;
    }
}

void Tetris::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

    m_pScenes[m_eCurrentScene]->Render(m_hBackDC);

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void Tetris::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    float halfWidth = (float)width / 2;
    float halfHeight = (float)height / 2;

    learning::SetScreenSize(width, height);

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);
}

void Tetris::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}