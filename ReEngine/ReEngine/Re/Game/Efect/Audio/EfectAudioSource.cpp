#include "EfectSoundSource.h"
#include <Re\Game\GameActor.h>

namespace Efect
{

	void SoundSource::onUpdate(sf::Time dt)
	{
		for (auto& it : sounds)
			it.setPosition(getOwner()->getPosition().x, getOwner()->getPosition().y, 0);

		for (auto& it : musics)
			it.setPosition(getOwner()->getPosition().x, getOwner()->getPosition().y, 0);
	}
	void SoundListener::onUpdate(sf::Time dt)
	{
		sf::Listener::setPosition(getOwner()->getPosition().x, getOwner()->getPosition().y, 0);
	}
}
