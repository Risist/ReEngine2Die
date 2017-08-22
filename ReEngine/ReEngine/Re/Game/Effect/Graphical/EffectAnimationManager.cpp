#include <Re\Game\Effect\Graphical\EffectAnimationManager.h>
#include <Re\Game\Effect\Graphical\EffectModel.h>

namespace Effect
{
	bool AnimationManager::canBeParent(Base * potentialParent) const
	{
		return dynamic_cast<Effect::Model*>(potentialParent);
	}
	void AnimationManager::onInit()
	{
		Super::onInit();
		modelsUpdate = &((Effect::Model*)getParent())->modelsUpdate;
	}
	void AnimationManager::onUpdate(sf::Time dt)
	{
		Super::onUpdate(dt);
		for (auto it = controllers.begin(); it < controllers.end(); ++it)
		{
			(*it)->onUpdateModel();
		}
	}

	void AnimationManager::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		Super::serialiseF(file, saver);

		/// TODO
	}

	void AnimationManager::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		Super::deserialiseF(file, loader);

		DATA_SCRIPT_MULTILINE(file, loader)
		{
			ResId resId = loader.load<ResId>("resId", 0);
			if (resId)
			{
				addAnimation(resId);
			}
			else
			{
				std::string path = loader.loadRaw("path", "");
				addAnimation(path.c_str());
			}
		}
	}

}