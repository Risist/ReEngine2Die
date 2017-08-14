#pragma once
#include <Re\Game\Effect\EffectBase.h>

namespace Effect
{
	class Transform : public Base
	{




	private:
		/// where the component is actually placed
		Vector2D positon;
		/// facing direction of actor
		Angle rotation;
	};

}
