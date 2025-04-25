#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include "RenderHelp.h"

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr);
}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

}

void PlayScene::Update(float deltaTime)
{
    
}

void PlayScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    // 배경 그리기
    RECT rcClient;
    GetClientRect(WindowFromDC(hDC), &rcClient);
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hDC, &rcClient, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    // SpriteSheet 가져오기
    renderHelp::SpriteSheet* pSpriteSheet = m_pGame->GetBricksSpriteSheet();
    if (!pSpriteSheet) return;

    // 테스트용 - 각 블록 타입을 그려봅니다
    int blockSize = 30;
    int startX = 100;
    int startY = 100;

    // I 블록 (청록색, 인덱스 0)
    pSpriteSheet->DrawSprite(hDC, 0, startX, startY, blockSize, blockSize);

    // J 블록 (파란색, 인덱스 1)
    pSpriteSheet->DrawSprite(hDC, 1, startX + blockSize, startY, blockSize, blockSize);

    // T 블록 (보라색, 인덱스 2)
    pSpriteSheet->DrawSprite(hDC, 2, startX + blockSize * 2, startY, blockSize, blockSize);

    // S 블록 (초록색, 인덱스 3)
    pSpriteSheet->DrawSprite(hDC, 3, startX + blockSize * 3, startY, blockSize, blockSize);

    // L 블록 (주황색, 인덱스 4)
    pSpriteSheet->DrawSprite(hDC, 4, startX, startY + blockSize, blockSize, blockSize);

    // O 블록 (노란색, 인덱스 5)
    pSpriteSheet->DrawSprite(hDC, 5, startX + blockSize, startY + blockSize, blockSize, blockSize);

    // Z 블록 (빨간색, 인덱스 6)
    pSpriteSheet->DrawSprite(hDC, 6, startX + blockSize * 2, startY + blockSize, blockSize, blockSize);

    // 테두리 블록 (회색, 인덱스 7)
    pSpriteSheet->DrawSprite(hDC, 7, startX + blockSize * 3, startY + blockSize, blockSize, blockSize);

    // 테트리스 모양 만들기 (T 모양)
    int tetrisX = 300;
    int tetrisY = 200;

    // T 블록으로 T 모양 그리기
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX, tetrisY, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX - blockSize, tetrisY + blockSize, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX, tetrisY + blockSize, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX + blockSize, tetrisY + blockSize, blockSize, blockSize);

    // I 블록으로 라인 그리기
    int lineX = 500;
    int lineY = 200;

    for (int i = 0; i < 4; i++) {
        pSpriteSheet->DrawSprite(hDC, 0, lineX + (i * blockSize), lineY, blockSize, blockSize);
    }

    // 작은 테트리스 보드 그리기
    int boardX = 100;
    int boardY = 300;
    int boardWidth = 6;
    int boardHeight = 6;

    // 테두리 그리기
    for (int x = 0; x < boardWidth; x++) {
        pSpriteSheet->DrawSprite(hDC, 7, boardX + (x * blockSize), boardY, blockSize, blockSize);
        pSpriteSheet->DrawSprite(hDC, 7, boardX + (x * blockSize), boardY + (boardHeight - 1) * blockSize, blockSize, blockSize);
    }

    for (int y = 1; y < boardHeight - 1; y++) {
        pSpriteSheet->DrawSprite(hDC, 7, boardX, boardY + (y * blockSize), blockSize, blockSize);
        pSpriteSheet->DrawSprite(hDC, 7, boardX + (boardWidth - 1) * blockSize, boardY + (y * blockSize), blockSize, blockSize);
    }

    // 몇 개의 블록 배치하기
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize, boardY + blockSize * 4, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize * 2, boardY + blockSize * 4, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize * 3, boardY + blockSize * 4, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize * 4, boardY + blockSize * 4, blockSize, blockSize);

    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize, boardY + blockSize * 3, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize * 2, boardY + blockSize * 3, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize * 2, boardY + blockSize * 2, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize * 3, boardY + blockSize * 2, blockSize, blockSize);

    // 디버그 정보 표시
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(0, 0, 0));

    RECT textRect = { 10, 10, rcClient.right - 10, 40 };
    DrawText(hDC, L"SpriteSheet 테스트 - 테트리스 블록 렌더링", -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    wchar_t infoText[100];
    wsprintfW(infoText, L"스프라이트 개수: %d", pSpriteSheet->GetSpriteCount());

    textRect = { 10, 40, rcClient.right - 10, 70 };
    DrawText(hDC, infoText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void PlayScene::Finalize()
{
   
}

void PlayScene::Enter()
{
    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!

}

void PlayScene::Leave()
{
}