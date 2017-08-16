#include <Re\Game\Effect\EffectBase.h>
#include "EffectBase.h"
namespace Effect
{
	Base::Base()
		:
		bActivated(true),
		bIsReadyToRemove(false)
	{
	}

	void Base::serialiseF(std::ostream& file, Res::DataScriptSaver& saver)  const
	{
		saver.save("effectType", getName() );
		saver.save<bool>("activated", bActivated, true);
		saver.nextLine(file);
		
		auto it = childs.begin();
		do
		{
			(*it)->serialise(file, saver);
			++it;
		}
		DATA_SCRIPT_MULTILINE_SAVE(file, saver,it != childs.end() )
	}
	void Base::deserialiseF(std::istream& file, Res::DataScriptLoader& loader)
	{
		setActivated(loader.load<bool>("activated", true));
		DATA_SCRIPT_MULTILINE(file, loader)
		{
			const string type = loader.load<string>("effectType", "Base");
			Base* _new = creationFunction(type);
			if (_new)
				addEffect(_new)->deserialise(file, loader);
			else
				cerr << "AddEffect failed due to invalid effect type: " << type << endl;
		}
	}

	////////////
	
	/// @summary: called after owner ptr is set up
	void Base::onInit()
	{
		/// do not call on childs
	}

	/// called once when actor is added to world list
	/// or when efect is inserted to already added actor
	void Base::onSpawn()
	{
		for (auto it = childs.begin(); it != childs.end(); ++it)
			it->get()->onSpawn();
	}

	void Base::onSetActivated()
	{
		for (auto it = childs.begin(); it != childs.end(); ++it)
			it->get()->onSetActivated();
	}


	/// called every frame while actor is alive
	/// @param:dt			- delta of time elapsed between frames
	void Base::onUpdate(sf::Time dt)
	{
		/// remove childs 
		for (auto it = childs.begin(); it != childs.end();)
			if (it->get()->bIsReadyToRemove)
				childs.erase(it++);
			else
				++it;

		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->bActivated)
				it->get()->onUpdate(dt);
	}

	void Base::onPause(sf::Time dt)
	{
		/// remove childs 
		for (auto it = childs.begin(); it != childs.end();)
			if (it->get()->bIsReadyToRemove)
				childs.erase(it++);
			else
				++it;

		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->bActivated)
				it->get()->onUpdate(dt);
	}


	/// @summary: event called every frame while actor is assigned as dead
	/// @param:dt			- delta of time elapsed between frames
	/// @return:			- whether owner can be safetly removed from memory
	bool Base::onDeath(sf::Time dt)
	{
		/// remove childs 
		for (auto it = childs.begin(); it != childs.end();)
			if (it->get()->bIsReadyToRemove)
				childs.erase(it++);
			else
				++it;


		bool b = true;
		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->bActivated)
				b = b && it->get()->onDeath(dt);
		return b;
	}


	/// @summary: event called when collision just started
	/// @param:otherActor	- actor owner is colliding with
	/// @param:contact		- box2d provided data about collision
	void Base::onCollisionEnter(Game::Actor& otherActor, b2Contact& contact)
	{
		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->canBeUpdated())
				it->get()->onCollisionEnter(otherActor, contact);
	}

	/// @summary: event called every frame 
	/// @param:dt			- delta of time elapsed between frames of collision update
	/// @param:otherActor	- actor owner is colliding with
	/// @param:contact		- box2d provided data about collision
	/// TODO virtual void onCollisionUpdate(sf::Time dt, Game::Actor& otherActor, b2Contact& contact)

	/// @summary: event called when collision just ended
	/// @param:otherActor	- actor owner is colliding with
	/// @param:contact		- box2d provided data about collision
	void Base::onCollisionExit(Game::Actor& otherActor, b2Contact& contact)
	{
		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->canBeUpdated())
				it->get()->onCollisionExit(otherActor, contact);
	}

	/// allows to filter collision detection
	/// @param:myFixture/otherFixture 
	///						- fixtures(box2d colliders) to check
	/// @return is collision allowed to happen? 
	bool Base::shouldCollide(b2Fixture* myFixture, b2Fixture* otherFixture)
	{
		bool b = true;
		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->canBeUpdated())
				b = b && it->get()->shouldCollide(myFixture, otherFixture);
		return b;
	}

	void Base::onPostSolve(Game::Actor& otherActor, b2Contact & contact, const b2ContactImpulse & impulse)
	{
		for (auto it = childs.begin(); it != childs.end(); ++it)
			if (it->get()->canBeUpdated())
				it->get()->onPostSolve(otherActor, contact, impulse);
	}


	void Base::setActivated(bool b) 
	{ 
		if (b == bActivated)
			return;
		
		bActivated = b; 
		onSetActivated();
	}
}