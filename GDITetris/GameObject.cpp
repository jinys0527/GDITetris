#include "INC_Windows.h"

#include "RenderHelp.h"
#include "Collider.h"
#include "GameObject.h"
#include <assert.h>

GameObject::~GameObject()
{
    if (m_pCollider)
    {
        delete m_pCollider;
        m_pCollider = nullptr;
    }
}

void GameObject::Update(float deltaTime)
{
    Move(deltaTime);
    UpdateFrame(deltaTime);
    // Collider ������Ʈ
    if (m_pCollider)
    {
        m_pCollider->SetCenter(m_pos);
    }
}

void GameObject::Render(HDC hdc, COLORREF color)
{
    DrawBitmap(hdc);
    DrawCollider(hdc, color);
}


void GameObject::SetColliderCircle(float radius)
{
    if (m_pCollider)
    {
        delete m_pCollider;
        m_pCollider = nullptr;
    }

    m_pCollider = new ColliderCircle();

    assert(m_pCollider != nullptr && "Failed to create ColliderCircle!");
    ColliderCircle* m_pColliderCircle = dynamic_cast<ColliderCircle*>(m_pCollider);

    if (m_pColliderCircle != nullptr)
    {
        m_pColliderCircle->SetRadius(radius);
    }
    m_pCollider->SetCenter(m_pos);
}


void GameObject::SetColliderBox(float width, float height)
{
    if (m_pCollider)
    {
        delete m_pCollider;
        m_pCollider = nullptr;
    }

    m_pCollider = new ColliderBox();

    assert(m_pCollider != nullptr && "Failed to create ColliderBox!");

    ColliderBox* m_pColliderBox = dynamic_cast<ColliderBox*>(m_pCollider);

    if (m_pColliderBox != nullptr)
    {
        m_pColliderBox->SetHalfSize(width / 2.0f, height / 2.0f);
    }
    m_pCollider->SetCenter(m_pos);
}

void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

    m_pBitmapInfo = bitmapInfo;

    // ��������Ʈ ������ �ϴ��� �ϵ��ڵ��ؿ�. 
    // �ϴ�, ������ ũ��� �ð��� ���ٰ� �����մϴ�.
    m_frameWidth = m_pBitmapInfo->GetWidth() / 5;
    m_frameHeight = m_pBitmapInfo->GetHeight() / 3;
    m_frameIndex = 0;

    for (int i = 0; i < 5; ++i)
    {
        m_frameXY[i].x = i * m_frameWidth;
        m_frameXY[i].y = 0;
    }

    for (int i = 0; i < 5; ++i)
    {
        m_frameXY[i + 5].x = i * m_frameWidth;
        m_frameXY[i + 5].y = m_frameHeight;
    }

    for (int i = 0; i < 4; ++i)
    {
        m_frameXY[i + 10].x = i * m_frameWidth;
        m_frameXY[i + 10].y = m_frameHeight * 2;
    }
}

learning::Collider* GameObject::GetCollider()
{
    return m_pCollider;
}

bool GameObject::Intersect(Collider* const& lhs, Collider* const& rhs)
{
    return lhs->Intersect(lhs, rhs);
}

// ����: �ش� �ڵ��� ������ �����ϱ��? ��� �����ϸ� �������?
// ���� ���⿡ ���� ���� ����ϰ� ���� ������.
void GameObject::DrawCollider(HDC hdc, COLORREF color)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    if (m_pCollider)
    {
        m_pCollider->DrawCollider(hdc);
    }

    // ���� ��ü ���� �� �� ����
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

void GameObject::Move(float deltaTime)
{
    GameObjectBase::Move(deltaTime);
}

void GameObject::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

    HDC hBitmapDC = CreateCompatibleDC(hdc);

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION ���� (���� ä�� ó��)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // ���� ���� ä�� �״�� ���
    blend.AlphaFormat = AC_SRC_ALPHA;

    const int x = m_pos.x - m_width / 2;
    const int y = m_pos.y - m_height / 2;

    const int srcX = m_frameXY[m_frameIndex].x;
    const int srcY = m_frameXY[m_frameIndex].y;

    AlphaBlend(hdc, x, y, m_width, m_height,
        hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);

    // ��Ʈ�� �ڵ� ����
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}

void GameObject::UpdateFrame(float deltaTime)
{
    m_frameTime += deltaTime;
    if (m_frameTime >= m_frameDuration)
    {
        m_frameTime = 0.0f;
        m_frameIndex = (m_frameIndex + 1) % m_frameCount;
    }
}

void GameObjectBase::SetName(const char* name)
{
    if (name == nullptr) return;

    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}

Background::~Background() 
{
}

void Background::Update(float deltaTime)
{
    // ����� ������Ʈ�� �ʿ����
       // Ȥ�� ����� ������ ��쿡�� �̰��� �����ϸ� ��
}

void Background::Render(HDC hdc, COLORREF color)
{
    DrawBitmap(hdc);
}

void Background::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

    m_pBitmapInfo = bitmapInfo;
}

void Background::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
    HDC hBitmapDC = CreateCompatibleDC(hdc);

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION ���� (���� ä�� ó��)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // ���� ���� ä�� �״�� ���
    blend.AlphaFormat = AC_SRC_ALPHA;

    int screenWidth = 0;
    int screenHeight = 0;
    learning::GetScreenSize(screenWidth, screenHeight);

    AlphaBlend(hdc, 0, 0, screenWidth, screenHeight,
        hBitmapDC, 0, 0, m_width, m_height, blend);
    // ��Ʈ�� �ڵ� ����
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}
