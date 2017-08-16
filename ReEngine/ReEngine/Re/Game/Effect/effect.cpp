#include <Re\Game\Effect\effects.h>

namespace Effect
{
	/// define function which will be used for serialisation of all standard efects
	MULTI_SERIALISATION_INTERFACE_IMPL(Base)
	{
		MULTI_SERIALISATION_INTERFACE_CHECK(Transformable);
		
		return nullptr;
	}
}