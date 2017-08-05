#include <Re\Ai\Mind\AiBehaviours.h>

namespace Ai
{
	

	//////////////////////////////// Emotion
	Utility_t BehaviourEmotion::getUtility() const
	{
		assert(ownersMood);
		return baseUtility + scaleUtility *behaviourEmotion.compare(*ownersMood);
	}

	void BehaviourEmotion::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		behaviourEmotion.serialise(file, saver);

		saver.save("baseUtility", baseUtility);
		saver.save("scaleUtility", scaleUtility);
	}

	void BehaviourEmotion::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		behaviourEmotion.deserialise(file, loader);

		baseUtility = loader.load("baseUtility", 0.f);
		scaleUtility = loader.load("scaleUtility", 1.f);
	}

	////////////////////////////////////////////// Wait

	void BehaviourWait::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		saver.save("waitTime", waitTime.asSeconds());
		saver.save("utility", utility);
	}

	void BehaviourWait::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		waitTime = seconds(loader.load("waitTime", 1.f ));
		utility = loader.load("utility", 1.f);
	}

}