#include "Collider.h"
#include <cmath>

using namespace learning;

double ColliderCircle::GetRadius()
{
	return m_radius;
}

Vector2f Collider::GetCenter()
{
	return m_center;
}

Vector2f ColliderBox::GetHalfSize()
{
	return m_halfSize;
}

bool ColliderCircle::Intersect(Collider* const& lhs, Collider* const& rhs)
{
	ColliderCircle* lColliderCircle = dynamic_cast<ColliderCircle*>(lhs);
	ColliderCircle* rColliderCircle = dynamic_cast<ColliderCircle*>(rhs);
	if (lColliderCircle != nullptr && rColliderCircle != nullptr)
	{
		return ((*rColliderCircle).GetCenter() - (*lColliderCircle).GetCenter()).LengthSquared() <= pow((*lColliderCircle).GetRadius() + (*rColliderCircle).GetRadius(), 2);
	}
	ColliderBox* rColliderBox = dynamic_cast<ColliderBox*>(rhs);
	if (lColliderCircle != nullptr && rColliderBox != nullptr)
	{
		float closestX = std::fmax(rColliderBox->GetCenter().x - rColliderBox->GetHalfSize().x, std::fmin(lColliderCircle->GetCenter().x, rColliderBox->GetCenter().x + rColliderBox->GetHalfSize().x));
		float closestY = std::fmax(rColliderBox->GetCenter().y - rColliderBox->GetHalfSize().y, std::fmin(lColliderCircle->GetCenter().y, rColliderBox->GetCenter().y + rColliderBox->GetHalfSize().y));
		
		float distanceX = fabs(lColliderCircle->GetCenter().x - closestX);
		float distanceY = fabs(lColliderCircle->GetCenter().y - closestY);
		float distancePoint = pow(distanceX, 2) + pow(distanceY, 2);

		return distancePoint <= lColliderCircle->GetRadius() * lColliderCircle->GetRadius();
	}
}

void ColliderCircle::DrawCollider(HDC hdc)
{
	Ellipse(hdc, this->GetCenter().x - this->GetRadius(),
		this->GetCenter().y - this->GetRadius(),
		this->GetCenter().x + this->GetRadius(),
		this->GetCenter().y + this->GetRadius());
}



bool ColliderBox::Intersect(Collider* const& lhs, Collider* const& rhs)
{
	ColliderBox* lColliderBox = dynamic_cast<ColliderBox*>(lhs);
	ColliderBox* rColliderBox = dynamic_cast<ColliderBox*>(rhs);

	if (lColliderBox != nullptr && rColliderBox != nullptr)
	{
		//x-direction
		if ((*lColliderBox).GetCenter().x - (*lColliderBox).GetHalfSize().x > (*rColliderBox).GetCenter().x + (*rColliderBox).GetHalfSize().x) return false;
		if ((*lColliderBox).GetCenter().x + (*lColliderBox).GetHalfSize().x < (*rColliderBox).GetCenter().x - (*rColliderBox).GetHalfSize().x) return false;

		//y-direction
		if ((*lColliderBox).GetCenter().y - (*lColliderBox).GetHalfSize().y > (*rColliderBox).GetCenter().y + (*rColliderBox).GetHalfSize().y) return false;
		if ((*lColliderBox).GetCenter().y + (*lColliderBox).GetHalfSize().y < (*rColliderBox).GetCenter().y - (*rColliderBox).GetHalfSize().y) return false;

		return true;
	}

	ColliderCircle* rColliderCircle = dynamic_cast<ColliderCircle*>(rhs);
	if (lColliderBox != nullptr && rColliderCircle != nullptr)
	{
		return rhs->Intersect(rhs, lhs);
	}
	
}

void ColliderBox::DrawCollider(HDC hdc)
{
	Rectangle(hdc, this->GetCenter().x - this->GetHalfSize().x,
		this->GetCenter().y - this->GetHalfSize().y,
		this->GetCenter().x + this->GetHalfSize().x,
		this->GetCenter().y + this->GetHalfSize().y);
}


