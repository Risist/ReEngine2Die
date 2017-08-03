#include <Re\Ai\Mind\AiMind.h>

namespace Ai
{
	Mind::Mind()
	{
		for (int i = 0; i < MEMORY_SIZE; ++i)
			behaviourMemory[i] = nullptr;
	}


	void Mind::onUpdate(sf::Time dt)
	{
		if (actualBehaviour && 
			/// execute behaviour
			actualBehaviour->onExecute(dt) )
			/// set up new behaviour 
		{
			/// fill chances vector
			assert(behaviours.size() == chance.chances.size());
			size_t size = behaviours.size();
			Utility_t helperValue;
			for (int i = 0; i < size; ++i)
			{
				helperValue = behaviours[i]->getUtility();
				chance.chances[i] = helperValue > treshold ? helperValue : 0;
			}

			/// choose new behaviour
			size_t id = chance.randId();
			actualBehaviour->onExit();
			actualBehaviour = behaviours[id].get();
			actualBehaviour->onStart();

			/// update memory
			behaviourMemory[4] = behaviourMemory[3];
			behaviourMemory[3] = behaviourMemory[2];
			behaviourMemory[2] = behaviourMemory[1];
			behaviourMemory[1] = behaviourMemory[0];
			behaviourMemory[0] = actualBehaviour;
		}

	}

}