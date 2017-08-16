#include <Re\Game\Effect\effects.h>



namespace Effect
{
	/// define function which will be used for serialisation of all standard efects
	MULTI_SERIALISATION_INTERFACE_IMPL(Base)
	{
		/// general
		MULTI_SERIALISATION_INTERFACE_CHECK(Transformable);
		MULTI_SERIALISATION_INTERFACE_CHECK(Base);

		/// graphics
		if (name == "DebugCircle") return new Debug::GraphicsCircle;


		return nullptr;
	}
}