#include<Re\Common\Control\ControlAxis.h>

namespace Control
{
	float32 AxisKey::getValue() const
	{
		assert(positiveKey && negativeKey);
		return (positiveKey->isReady() - negativeKey->isReady());
	}
}