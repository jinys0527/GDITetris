#pragma once
#include "Utillity.h"
#include <windows.h>

namespace learning
{
	class Collider
	{
	public:
		Collider() {}
		virtual bool Intersect(Collider* const& lhs, Collider* const& rhs) = 0;
		virtual void DrawCollider(HDC hdc) = 0;
		Vector2f GetCenter();
		void SetCenter(Vector2f center) { m_center = center; }
	protected:
		Vector2f m_center;
	};

	class ColliderCircle : public Collider
	{
	public:
		ColliderCircle() : Collider(){}
		virtual bool Intersect(Collider* const& lhs, Collider* const& rhs) override;
		void DrawCollider(HDC hdc) override;
		double GetRadius();
		void SetRadius(double radius) { m_radius = radius; }
	private:
		double m_radius;
	};

	class ColliderBox : public Collider
	{
	public:
		ColliderBox() : Collider() {}
		virtual bool Intersect(Collider* const& lhs, Collider* const& rhs) override;
		void DrawCollider(HDC hdc) override;
		Vector2f GetHalfSize();
		void SetHalfSize(float x, float y) { m_halfSize.x = x; m_halfSize.y = y; }
	private:
		Vector2f m_halfSize;
	};
}

