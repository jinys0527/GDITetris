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

    // ��� �׸���
    RECT rcClient;
    GetClientRect(WindowFromDC(hDC), &rcClient);
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hDC, &rcClient, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    // SpriteSheet ��������
    renderHelp::SpriteSheet* pSpriteSheet = m_pGame->GetBricksSpriteSheet();
    if (!pSpriteSheet) return;

    // �׽�Ʈ�� - �� ��� Ÿ���� �׷����ϴ�
    int blockSize = 30;
    int startX = 100;
    int startY = 100;

    // I ��� (û�ϻ�, �ε��� 0)
    pSpriteSheet->DrawSprite(hDC, 0, startX, startY, blockSize, blockSize);

    // J ��� (�Ķ���, �ε��� 1)
    pSpriteSheet->DrawSprite(hDC, 1, startX + blockSize, startY, blockSize, blockSize);

    // T ��� (�����, �ε��� 2)
    pSpriteSheet->DrawSprite(hDC, 2, startX + blockSize * 2, startY, blockSize, blockSize);

    // S ��� (�ʷϻ�, �ε��� 3)
    pSpriteSheet->DrawSprite(hDC, 3, startX + blockSize * 3, startY, blockSize, blockSize);

    // L ��� (��Ȳ��, �ε��� 4)
    pSpriteSheet->DrawSprite(hDC, 4, startX, startY + blockSize, blockSize, blockSize);

    // O ��� (�����, �ε��� 5)
    pSpriteSheet->DrawSprite(hDC, 5, startX + blockSize, startY + blockSize, blockSize, blockSize);

    // Z ��� (������, �ε��� 6)
    pSpriteSheet->DrawSprite(hDC, 6, startX + blockSize * 2, startY + blockSize, blockSize, blockSize);

    // �׵θ� ��� (ȸ��, �ε��� 7)
    pSpriteSheet->DrawSprite(hDC, 7, startX + blockSize * 3, startY + blockSize, blockSize, blockSize);

    // ��Ʈ���� ��� ����� (T ���)
    int tetrisX = 300;
    int tetrisY = 200;

    // T ������� T ��� �׸���
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX, tetrisY, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX - blockSize, tetrisY + blockSize, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX, tetrisY + blockSize, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 2, tetrisX + blockSize, tetrisY + blockSize, blockSize, blockSize);

    // I ������� ���� �׸���
    int lineX = 500;
    int lineY = 200;

    for (int i = 0; i < 4; i++) {
        pSpriteSheet->DrawSprite(hDC, 0, lineX + (i * blockSize), lineY, blockSize, blockSize);
    }

    // ���� ��Ʈ���� ���� �׸���
    int boardX = 100;
    int boardY = 300;
    int boardWidth = 6;
    int boardHeight = 6;

    // �׵θ� �׸���
    for (int x = 0; x < boardWidth; x++) {
        pSpriteSheet->DrawSprite(hDC, 7, boardX + (x * blockSize), boardY, blockSize, blockSize);
        pSpriteSheet->DrawSprite(hDC, 7, boardX + (x * blockSize), boardY + (boardHeight - 1) * blockSize, blockSize, blockSize);
    }

    for (int y = 1; y < boardHeight - 1; y++) {
        pSpriteSheet->DrawSprite(hDC, 7, boardX, boardY + (y * blockSize), blockSize, blockSize);
        pSpriteSheet->DrawSprite(hDC, 7, boardX + (boardWidth - 1) * blockSize, boardY + (y * blockSize), blockSize, blockSize);
    }

    // �� ���� ��� ��ġ�ϱ�
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize, boardY + blockSize * 4, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize * 2, boardY + blockSize * 4, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize * 3, boardY + blockSize * 4, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 0, boardX + blockSize * 4, boardY + blockSize * 4, blockSize, blockSize);

    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize, boardY + blockSize * 3, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize * 2, boardY + blockSize * 3, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize * 2, boardY + blockSize * 2, blockSize, blockSize);
    pSpriteSheet->DrawSprite(hDC, 6, boardX + blockSize * 3, boardY + blockSize * 2, blockSize, blockSize);

    // ����� ���� ǥ��
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(0, 0, 0));

    RECT textRect = { 10, 10, rcClient.right - 10, 40 };
    DrawText(hDC, L"SpriteSheet �׽�Ʈ - ��Ʈ���� ��� ������", -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    wchar_t infoText[100];
    wsprintfW(infoText, L"��������Ʈ ����: %d", pSpriteSheet->GetSpriteCount());

    textRect = { 10, 40, rcClient.right - 10, 70 };
    DrawText(hDC, infoText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void PlayScene::Finalize()
{
   
}

void PlayScene::Enter()
{
    // [CHECK]. ù ��° ���� ������Ʈ�� �÷��̾� ĳ���ͷ� ����!

}

void PlayScene::Leave()
{
}