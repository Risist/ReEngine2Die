#pragma once

#include <Re\Game\Efect\EfectBase.h>
#include <Re\Common\utility.h>

namespace Efect
{
	/// efect creates rigidbody and after that removes self
	class Rigidbody : public Base
	{
	public:
		/// create static Rigidbody
		Rigidbody();
		/// Dynamic & kinematic
		Rigidbody(float32 linearDamping, float32 angularDamping,
			bool fixedRotation = false, bool bullet = false, bool allowSleep = true,
			b2BodyType type = b2_dynamicBody);

		void onInit() override;

		Rigidbody* setType(b2BodyType s)
		{
			def.type = s;
			return this;
		}
		Rigidbody* setDamping(float32 linear, float32 angular)
		{
			def.linearDamping = linear;
			def.angularDamping = angular;
			return this;
		}
		Rigidbody* setFixedRotation(bool s)
		{
			def.fixedRotation = s;
			return this;
		}
		Rigidbody* setIsBullet(bool s)
		{
			def.bullet = s;
			return this;
		}
		Rigidbody* setAllowSleep(bool s)
		{
			def.allowSleep = s;
			return this;
		}
		Rigidbody* setVelocity(const Vector2D& linear, Angle angular)
		{
			def.angularVelocity = angular.asRadian();
			def.linearVelocity = linear*toB2Position;
			return this;
		}
		Rigidbody* setTransform(const Vector2D& position, Angle rotation)
		{
			def.position = position*toB2Position;
			def.angle = rotation.asRadian();
			return this;
		}

		b2BodyDef def;
	};


}