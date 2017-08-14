#include <Re\Game\Efect\Physics\EfectCollider.h>
#include <Re\Game\GameActor.h>

namespace Efect
{
	ColliderCircle::ColliderCircle(float32 radius, float32 density, Vector2D offset, bool isSensor, 
		float32 restitution, float32 friction)
	{
		shapeDef.m_radius = radius * toB2Position;
		shapeDef.m_p = offset * toB2Position;

		fixtureDef.shape = &shapeDef;
		fixtureDef.density = density;
		fixtureDef.isSensor = isSensor;
		fixtureDef.restitution = restitution;
		fixtureDef.friction = friction;

	}
	void ColliderCircle::onStart()
	{
		assert(getOwner()->isRigidbodyCreated());
		fixture = getOwner()->getRigidbody().CreateFixture(&fixtureDef);
		readyToRemove = true;
	}
	/////////////////////////////////////////////
	
	ColliderBox::ColliderBox(Vector2D wh, float32 density, bool isSensor, float32 restitution, float32 friction)
	{
		shapeDef.SetAsBox(wh.x * toB2Position, wh.y * toB2Position);

		fixtureDef.shape = &shapeDef;
		fixtureDef.density = density;
		fixtureDef.isSensor = isSensor;
		fixtureDef.restitution = restitution;
		fixtureDef.friction = friction;
	}

	void ColliderBox::onStart()
	{
		assert(getOwner()->isRigidbodyCreated());
		if (points.empty() == false)
			shapeDef.Set(points.data(), points.size());
		fixture = getOwner()->getRigidbody().CreateFixture(&fixtureDef);
		readyToRemove = true;
	}
}