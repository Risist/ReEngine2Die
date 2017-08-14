#include <Re\Game\Efect\Physics\EfectUpdateTransform.h>
#include <Re\Game\GameActor.h>

namespace Efect
{

	UpdateTransform::UpdateTransform(Mode _positionMode, Mode _rotationMode)
		: positionMode(_positionMode), rotationMode(_rotationMode)
	{
	}

	UpdateTransform::UpdateTransform(Mode _wholeMode)
		:positionMode(_wholeMode), rotationMode(_wholeMode)
	{
	}

	void UpdateTransform::onInit()
	{
		Base::onInit();
		transformOwner = getOwner();
		rigidbody = &getOwner()->getRigidbody();
	}

	void UpdateTransform::onUpdate(sf::Time dt)
	{
		switch (positionMode)
		{
		case Efect::UpdateTransform::RigidbodyToTransform:
			{	
				assert(rigidbody && transformOwner);

				Vector2D desiredPosition = transformOwner->getPosition()* toB2Position;
				Vector2D toDesiredPosition = desiredPosition - (Vector2D)rigidbody->GetPosition();
				rigidbody->SetLinearVelocity(toDesiredPosition *updateRate);
			}
			break;
		case Efect::UpdateTransform::TransformToRigidbody:
			assert(rigidbody && transformOwner); 
			transformOwner->setPosition((Vector2D)getOwner()->getRigidbody().GetPosition() * toSfPosition);
			break;
		}


		switch (rotationMode)
		{
		case Efect::UpdateTransform::RigidbodyToTransform:
			assert(rigidbody && transformOwner);
			{

				Angle desiredRotation = Degree(transformOwner->getRotation());
				Angle toDesiredRotation = desiredRotation.minimalDiffirence(Radian(rigidbody->GetAngle()));
				rigidbody->SetAngularVelocity(toDesiredRotation.asRadian() * updateRate);
			}
			break;
		case Efect::UpdateTransform::TransformToRigidbody:
			assert(rigidbody && transformOwner);
			getOwner()->setRotation(Radian(getOwner()->getRigidbody().GetAngle()).asDegree());
			break;
		}
	}
}
