#pragma once

#include <Re\Game\Efect\EfectBase.h>

namespace Efect
{
	/// efect which invokes update of transform to fit rigidbody every frame
	/// there are sevral update modes
	/// @Warring before first update of this efect 
	///		Actor have to have an rigidbody created
	class UpdateTransform : public Base
	{
	public:
		enum Mode
		{
			none,
			RigidbodyToTransform,
			TransformToRigidbody,
		} positionMode, rotationMode;
		UpdateTransform(Mode _positionMode, Mode _rotationMode);
		UpdateTransform(Mode _wholeMode= TransformToRigidbody);

		virtual void onInit() override;
		virtual void onUpdate(sf::Time dt) override;


		/// transform to which update phisics
		/// by default component owner
		UpdateTransform* setTransformOwner(Transformable* s)
		{
			transformOwner = s;
			return this;
		}
		/// rigidbody to which 
		/// by default owners rigidbody (nullptr if not created yet)
		UpdateTransform* setRigidbody(b2Body* s)
		{
			rigidbody= s;
			return this;
		}
		UpdateTransform* setUpdateRate(float32 s)
		{
			updateRate = s;
			return this;
		}

		Transformable *transformOwner;
		b2Body *rigidbody;
		float32 updateRate{ 60 };
	};

}