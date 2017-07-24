#pragma once
#include <Re\Game\Efect\EfectBase.h>
#include <Re\Graphics\Model\GraphicsAnimationController.h>
#include <Re\Graphics\ResourceManager.h>

namespace Efect
{
	/// class to maintain animations
	/// since animation need to be updated separately from models
	/// this component is able does the job
	///
	/// class as well allows to turn on/off choosen animation
	class AnimationManager : public Base
	{
	public:
		AnimationManager(vector<Graphics::Model*>& _modelsUpdate)
			: modelsUpdate(_modelsUpdate)
		{
		}
		virtual void onUpdate(sf::Time dt) override;

		AnimationManager* addAnimation(ResId scriptId, bool activated = true)
		{
			controllers.push_back(make_unique<Graphics::AnimationController>());
			controllers.back()->deserialiseFromString(scriptInst[scriptId]);
			controllers.back()->attachToModel(modelsUpdate);
			controllersActivated.push_back(activated);
			return this;
		}
		AnimationManager* addAnimation(const char* path, bool activated = true)
		{
			controllers.push_back(make_unique<Graphics::AnimationController>(path));
			controllers.back()->attachToModel(modelsUpdate);
			controllersActivated.push_back(activated);
			return this;
		}

		/// disables all animation beside one specific
		AnimationManager* activateOneAnimation(size_t id);
		AnimationManager* activateAnimation(size_t id, bool activated = true)
		{
			assert(controllersActivated.size() < id);
			controllersActivated[id] = activated;
			return this;
		}
		Graphics::AnimationController& getAnimation(size_t id)
		{
			assert(controllers.size() < id);
			return *controllers[id];
		}
		Graphics::AnimationController& getAnimation()
		{
			return *controllers.back();
		}

	private:
		/// animations holded and managed in the class
		vector<unique_ptr<Graphics::AnimationController>> controllers;
		vector<bool> controllersActivated;
		vector<Graphics::Model*>& modelsUpdate;
	};


}