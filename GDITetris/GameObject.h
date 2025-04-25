#pragma once

#include "Utillity.h"
#include <algorithm>
// [CHECK]. namespace �����ؼ� ���� �����ؾ� ��
namespace learning
{
    class Collider;
    class ColliderCircle;
    class ColliderBox;
}

namespace renderHelp
{
    class BitmapInfo;
}


enum class ObjectType
{
    PLAYER,
    ENEMY,
    BULLET,
    ITEM,
    BACKGROUND,
};

constexpr int OBJECT_NAME_LEN_MAX = 15;

class GameObjectBase
{
    using Vector2f = learning::Vector2f;
public:
    GameObjectBase() = delete;
    GameObjectBase(const GameObjectBase&) = delete;

    GameObjectBase(ObjectType type) : m_type(type) {}

    virtual ~GameObjectBase() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(HDC hdc, COLORREF color) = 0;

    void SetPosition(float x, float y) { m_pos = { x, y }; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetName(const char* name);
    void SetColor(COLORREF color) { m_color = color; }
    void SetWidth(int width) { m_width = width; }
    void SetHeight(int height) { m_height = height; }

    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }
    const COLORREF GetColor() const { return m_color; }

    Vector2f GetPosition() const { return m_pos; }
    Vector2f GetDirection() const { return m_dir; }

    float GetSpeed() const { return m_speed; }

protected:

    void Move(float deltaTime)
    {
        m_pos.x += m_dir.x * m_speed * deltaTime;
        m_pos.y += m_dir.y * m_speed * deltaTime;
    }

protected:
    ObjectType m_type;

    Vector2f m_pos = { 0.0f, 0.0f };
    Vector2f m_dir = { 0.0f, 0.0f }; // ���� (���� ����)
    COLORREF m_color = RGB(255, 0, 0);
    float m_speed = 0.0f; // �ӷ�
    int m_width = 0;
    int m_height = 0;

    char m_name[OBJECT_NAME_LEN_MAX] = "";
};

class GameObject : public GameObjectBase
{
    using Collider = learning::Collider;
    using ColliderCircle = learning::ColliderCircle;
    using ColliderBox = learning::ColliderBox;
    using BitmapInfo = renderHelp::BitmapInfo;

public:
    GameObject(const GameObject&) = delete;
    GameObject(ObjectType type) : GameObjectBase(type) {}
    ~GameObject() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc, COLORREF color) override;

    void SetColliderCircle(float radius);
    void SetColliderBox(float halfWidth, float halfHeight);
    
    void SetBitmapInfo(BitmapInfo* bitmapInfo);

    Collider* GetCollider();

    bool Intersect(Collider* const& lhs, Collider* const& rhs);

protected:
    void DrawCollider(HDC hdc, COLORREF color);

    void Move(float deltaTime);

    // Collider
    Collider* m_pCollider = nullptr;

    void DrawBitmap(HDC hdc);
    void UpdateFrame(float deltaTime);

    // Bitmap ����
    BitmapInfo* m_pBitmapInfo = nullptr;

    // ���������� ���ڰ� ���� ���ƿ�.
    struct FrameFPos
    {
        int x;
        int y;
    };
    // ������ ����: �� 14���İ� �����ø� �ôٰ� �ۿ�...:)
    FrameFPos m_frameXY[14] = { { 0, 0 }, };
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 14; // ������ ��

    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f; // ���� ����
};

class Background : public GameObject
{
    using BitmapInfo = renderHelp::BitmapInfo;
public:
    Background(const Background&) = delete;
    Background(ObjectType type) : GameObject(type) {}
    ~Background() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc, COLORREF color) override;

    void SetBitmapInfo(BitmapInfo* bitmapInfo);

protected:
    void DrawBitmap(HDC hdc);
    BitmapInfo* m_pBitmapInfo = nullptr;
};