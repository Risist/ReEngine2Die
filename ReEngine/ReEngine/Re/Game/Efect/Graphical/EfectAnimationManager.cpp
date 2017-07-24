#include <Re\Game\Efect\Graphical\EfectAnimationManager.h>

namespace Efect
{

	void AnimationManager::onUpdate(sf::Time dt)
	{
		//controllers[0].onUpdateModel();
		//for (auto& it : controllers)
		//	it.onUpdateModel();

		auto itAc = controllersActivated.begin();
		for (auto it = controllers.begin(); it < controllers.end(); )
		{
			if (*itAc)
				(*it)->onUpdateModel();
			++it;
			++itAc;
		}
	}

	AnimationManager * AnimationManager::activateOneAnimation(size_t id)
	{
		for (auto& it : controllersActivated)
			it = false;
		controllersActivated[id] = true;
		return this;
	}

}