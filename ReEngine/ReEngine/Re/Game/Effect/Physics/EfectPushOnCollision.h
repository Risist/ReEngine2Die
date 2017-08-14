#pragma once

#include <Re\Game\Efect\EfectBase.h>
#include <Re\Common\utility.h>

namespace Efect
{

	class PushOnCollision  : public Base
	{
	public:

		virtual void onCollisionEnter(Game::Actor& otherActor, b2Contact& contact) override;
		virtual void onCollisionExit(Game::Actor& otherActor, b2Contact& contact) override;

		/////// setters
		PushOnCollision* setForce(const Vector2D& _force)
		{
			force = _force;
			return this;
		}
		PushOnCollision* setAllowed(function<bool(Game::Actor& otherActor, b2Contact& contact)>& s)
		{
			isAllowed = s;
			return this;
		}
		PushOnCollision* setRelativeToActor(bool s )
		{
			relativeToActor = s;
			return this;
		}
		PushOnCollision* setUseOnEnter(bool b)
		{
			useOnEnter = b;
			return this;
		}
		PushOnCollision* setUseOnExit(bool b)
		{
			useOnExit = b;
			return this;
		}

		/// used to filtering whether to apply force
		function<bool(Game::Actor& otherActor, b2Contact& contact)> isAllowed{ [](Game::Actor& otherActor, b2Contact& contact) { return true; } };
		/// what force to apply
		Vector2D force;
		/// has force relative rotation to owner?
		bool relativeToActor{true};
		bool useOnEnter{ true };
		///bool useOnUpdate{ false }; /// TODO
		bool useOnExit{ false };
	};

}