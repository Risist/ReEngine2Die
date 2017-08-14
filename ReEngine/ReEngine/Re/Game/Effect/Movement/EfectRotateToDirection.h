#pragma once
#include <Re\Game\Efect\EfectBase.h>
#include <limits>

namespace Efect
{

	/// utility efect (used by other efect)
	/// which rotates owner to given angle
	/// another efect should set up RotateToDirection::direction
	// field every change of desired direction of owner
	class RotateToDirection : public Base
	{
	public:
		enum Mode
		{
			instantTransform, /// instant change when no rigidbody applied
			smoothTransform, /// smooth change (lerp) when no rigidbody applied
			instantPhysics,  /// instant change when rigidbody is applied
			smoothPhysics, /// smooth change (lerp) when rigidbody is applied
		}mode;
		RotateToDirection(Mode mode = instantPhysics, float32 smoothAlpha = 1, Angle angleOffset = Angle() );

		void onUpdate(sf::Time dt) override;

		RotateToDirection* setMode(Mode s)
		{
			mode = s;
			return this;
		}
		RotateToDirection* setAngleOffset(Angle s)
		{
			angleOffset = s;
			return this;
		}
		RotateToDirection* setSmoothAlpha(float32 s)
		{
			smoothAlpha = s;
			return this;
		}
		RotateToDirection* setMaxRotationSpeed(Angle s)
		{
			maxRotationSpeed = s;
			return this;
		}

		Angle direction, angleOffset;
		float32 smoothAlpha;
		Angle maxRotationSpeed{Degree(numeric_limits<float32>::max())};
	};

}