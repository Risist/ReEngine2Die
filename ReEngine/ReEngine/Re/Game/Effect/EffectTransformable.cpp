#include <Re\Game\Effect\EffectTransformable.h>

namespace Effect
{
	void Transformable::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		saver.save<float>("posX", position.x, 0.f);
		saver.save<float>("posY", position.y, 0.f);
		saver.save<float>("rot", rotation.asDegree(), 0.f);
		Super::serialiseF(file, saver);
	}

	void Transformable::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		position = { 
			loader.load<float>("posX", 0.f),
			loader.load<float>("posY", 0.f) 
		};
		rotation = Degree(loader.load<float>("rot", 0.f));
		Super::deserialiseF(file, loader);
	}
}
