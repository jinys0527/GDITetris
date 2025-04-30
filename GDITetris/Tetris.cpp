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
#include "SoundManager.h"

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

    if (false == __super::Create(className, windowName, 1280, 960))
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
    m_pUIBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/ui.png");
    m_pKeyBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/keyboard.png");
    m_pButtonBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/button.png");
    m_pGameoverBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/gameover.png");

    m_pBricksSpriteSheet = new SpriteSheet();
    if (!m_pBricksSpriteSheet->LoadFromJson(L"../Resource/bricks.json") ||
        !m_pBricksSpriteSheet->LoadSprite(L"../Resource/bricks.png"))
    {
        std::cout << "SpriteSheet Load Failed!" << std::endl;
        return false;
    }

    if (m_pBackgroundBitmapInfo == nullptr || m_pUIBitmapInfo == nullptr
        || m_pKeyBitmapInfo == nullptr || m_pButtonBitmapInfo == nullptr 
        || m_pGameoverBitmapInfo == nullptr)
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

    if (!m_soundManager.Init())
    {
        return false;
    }

    m_soundManager.LoadSound(SoundManager::SOUND_MOVE, "../Sound/sfx_move.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_ROTATE, "../Sound/sfx_rotate.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_HARDDROP, "../Sound/sfx_harddrop.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_HOLD, "../Sound/sfx_hold.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_SINGLE, "../Sound/sfx_single.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_DOUBLE, "../Sound/sfx_double.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_TRIPLE, "../Sound/sfx_triple.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_TETRIS, "../Sound/sfx_tetris.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_TSPIN, "../Sound/sfx_tspin.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_PERFECTCLEAR, "../Sound/sfx_perfectclear.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO1, "../Sound/sfx_combo1.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO2, "../Sound/sfx_combo2.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO3, "../Sound/sfx_combo3.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO4, "../Sound/sfx_combo4.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO5, "../Sound/sfx_combo5.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO6, "../Sound/sfx_combo6.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_COMBO7, "../Sound/sfx_combo7.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_BACKTOBACK, "../Sound/sfx_b2b.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_GAMEOVER, "../Sound/sfx_gameover.wav", false);
    m_soundManager.LoadSound(SoundManager::SOUND_BGM, "../Sound/bgm.wav", true);

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
            else if (msg.message == WM_KEYUP)
            {
                m_pScenes[m_eCurrentScene]->OnKeyUp(msg.wParam);
            }
            else if (msg.message == WM_LBUTTONDOWN)
            {
                m_pScenes[m_eCurrentScene]->OnClicked(LOWORD(msg.lParam), HIWORD(msg.lParam));
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
            m_soundManager.Update();
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

BitmapInfo* Tetris::GetUIBitmapInfo() const
{
    return m_pUIBitmapInfo;
}

BitmapInfo* Tetris::GetKeyBitmapInfo() const
{
    return m_pKeyBitmapInfo;
}

BitmapInfo* Tetris::GetButtonBitmapInfo() const
{
    return m_pButtonBitmapInfo;
}

BitmapInfo* Tetris::GetGameoverBitmapInfo() const
{
    return m_pGameoverBitmapInfo;
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