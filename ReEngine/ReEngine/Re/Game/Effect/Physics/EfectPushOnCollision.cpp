#include <Re\Game\Efect\Physics\EfectPushOnCollision.h>
#include <Re\Game\GameActor.h>

namespace Efect
{
	void PushOnCollision::onCollisionEnter(Game::Actor & otherActor, b2Contact & contact)
	{
		if (useOnEnter && isAllowed(otherActor, contact))
		{
			assert(otherActor.isRigidbodyCreated());
			Vector2D f = force*toB2Position;
			if (relativeToActor)
				f.rotate(getOwner()->getRotation());
			otherActor.getRigidbody().ApplyForceToCenter(f,true);
		}
	}
	void PushOnCollision::onCollisionExit(Game::Actor & otherActor, b2Contact & contact)
	{
		if (useOnExit && isAllowed(otherActor, contact))
		{
			assert(otherActor.isRigidbodyCreated());
			Vector2D f = force*toB2Position;
			if (relativeToActor)
				f.rotate(getOwner()->getRotation());
			otherActor.getRigidbody().ApplyForceToCenter(f,true);
		}
	}
}