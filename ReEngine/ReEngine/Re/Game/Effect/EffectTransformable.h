#pragma once
#include <Re\Game\Effect\EffectBase.h>

namespace Effect
{


	/// efect with 
	class Transformable : public Base
	{
	public:

		Transformable(const Vector2D& _position = Vector2D(), Angle _rotation = Angle::zero)
			:position(_position), rotation(_rotation)	{}

		const Vector2D& getPosition() const { return position; }
		Angle getRotation() const { return rotation;  }

		void setPosition(const Vector2D& s) { position = s; }
		void setRotation(Angle s) { rotation = s; }

		///

		void translateTowards(const Vector2D& destination, float32 speed)
		{
			Vector2D toDestination = destination - position;
			position += toDestination.getNormalised()*speed;
		}
		bool translateTowards(const Vector2D& destination, float32 speed, float32 arriveRadius)
		{
			Vector2D toDestination = destination - position;
			if (toDestination.getLenghtSq() > arriveRadius*arriveRadius)
			{
				position += toDestination.getNormalised()*speed;
				return false;
			}
			else
				return true;
		}
		bool translateTowardsIdeal(const Vector2D& destination, float32 speed, float32 arriveRadius)
		{
			Vector2D toDestination = destination - position;
			if (toDestination.getLenghtSq() > arriveRadius*arriveRadius)
			{
				position += toDestination.getNormalised()*speed;
				return false;
			}
			else
			{
				position = destination;
				return true;
			}
		}

		void translateTowards(const Transformable& destination, float32 speed)
		{
			Vector2D toDestination = destination.position - position;
			position += toDestination.getNormalised()*speed;
		}
		bool translateTowards(const Transformable& destination, float32 speed, float32 arriveRadius)
		{
			Vector2D toDestination = destination.position - position;
			if (toDestination.getLenghtSq() > arriveRadius*arriveRadius)
			{
				position += toDestination.getNormalised()*speed;
				return false;
			}
			else
				return true;
		}
		bool translateTowardsIdeal(const Transformable& destination, float32 speed, float32 arriveRadius)
		{
			Vector2D toDestination = destination.position - position;
			if (toDestination.getLenghtSq() > arriveRadius*arriveRadius)
			{
				position += toDestination.getNormalised()*speed;
				return false;
			}
			else
			{
				position = destination.position;
				return true;
			}
		}

		///

		bool rotateTowards(Angle desired, Angle speed, Angle minDifference)
		{
			Angle toDesired = desired - rotation;
			if (toDesired > Angle::zero )
			{
				if (toDesired < minDifference)
				{
					rotation -= speed;
					return false;
				}
			}
			else
			{
				if (-toDesired < minDifference)
				{
					rotation += speed;
					return false;
				}
			}
			return true;
		}
		bool rotateTowardsIdeal(Angle desired, Angle speed, Angle minDifference)
		{
			Angle toDesired = desired - rotation;
			if (toDesired > Angle::zero)
			{
				if (toDesired < minDifference)
				{
					rotation -= speed;
					return false;
				}
			}
			else
			{
				if (-toDesired < minDifference)
				{
					rotation += speed;
					return false;
				}
			}

			rotation = desired;
			return true;
		}

		bool rotateTowards(const Vector2D& destination, Angle speed, Angle minDifference)
		{
			Vector2D toDestination = destination - position;
			return rotateTowards(toDestination.angle(), speed, minDifference);
		}
		bool rotateTowardsIdeal(const Vector2D& destination, Angle speed, Angle minDifference)
		{
			Vector2D toDestination = destination - position;
			return rotateTowardsIdeal(toDestination.angle(), speed, minDifference);
		}

		bool rotateTowards(const Transformable& destination, Angle speed, Angle minDifference)
		{
			Vector2D toDestination = destination.position - position;
			return rotateTowards(toDestination.angle(), speed, minDifference);
		}
		bool rotateTowardsIdeal(const Transformable& destination, Angle speed, Angle minDifference)
		{
			Vector2D toDestination = destination.position - position;
			return rotateTowardsIdeal(toDestination.angle(), speed, minDifference);
		}


	private:
		/// where the component is actually placed
		Vector2D position;
		/// facing direction of actor
		Angle rotation;
	protected:
		/// serialisation functions - to allow effects to be sierialised from/into .res files
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver)  const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

}
