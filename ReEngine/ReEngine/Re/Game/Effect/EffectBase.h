#pragma once
#include <Re\Common\utility.h>

namespace Game
{
	class Actor;
}

namespace Effect
{
	/// component-like class for Actors
	class Base : public Res::ISerialisable
	{
	public:
		Base();
		Base(const Base&) = delete;
		virtual ~Base() = default;

	public: /// events

		/// @summary: called after owner ptr is set up
		virtual void onInit();

		/// called once when owner is added to world list
		virtual void onSpawn();

		/// called when setActivated changes activation state of the effect
		virtual void onSetActivated();
		
		/// called every frame while actor is alive
		/// @param:dt			- delta of time elapsed between frames
		virtual void onUpdate(sf::Time dt);

		/// called every frame while game is paused
		/// @param:dt			- delta of time elapsed between frames
		virtual void onPause(sf::Time dt);

		
		/// @summary: event called every frame while actor is assigned as dead
		/// @param:dt			- delta of time elapsed between frames
		/// @return:			- whether owner can be safetly removed from memory
		virtual bool onDeath(sf::Time dt);


		/// @summary: event called when collision just started
		/// @param:otherActor	- actor owner is colliding with
		/// @param:contact		- box2d provided data about collision
		virtual void onCollisionEnter(Game::Actor& otherActor, b2Contact& contact);
		
		/// @summary: event called every frame 
		/// @param:dt			- delta of time elapsed between frames of collision update
		/// @param:otherActor	- actor owner is colliding with
		/// @param:contact		- box2d provided data about collision
		/// TODO virtual void onCollisionUpdate(sf::Time dt, Game::Actor& otherActor, b2Contact& contact)
		
		/// @summary: event called when collision just ended
		/// @param:otherActor	- actor owner is colliding with
		/// @param:contact		- box2d provided data about collision
		virtual void onCollisionExit(Game::Actor& otherActor, b2Contact& contact);
		
		/// allows to filter collision detection
		/// @param:myFixture/otherFixture 
		///						- fixtures(box2d colliders) to check
		/// @return is collision allowed to happen? 
		virtual bool shouldCollide(b2Fixture* myFixture, b2Fixture* otherFixture);

		/// @summary: event called after collision were resolved
		///		for more informations go to b2WorldCallbacks.h file
		/// @param:contact		- box2d provided data about collision
		/// @param:impulse		- data about applied impulse
		virtual void onPostSolve(Game::Actor& otherActor, b2Contact& contact, const b2ContactImpulse& impulse);

		bool canBeUpdated() { return !bIsReadyToRemove && bActivated; }

	public:/// childs
		/// function creating effect depended on its name 
		/// for purpose of creating in scripts
		/// if you create your own gameplay-depended effects provide new creation function
		/// in which if creation by default version will fail (return nullptr)
		/// you will check whether you chave class appropriate for the name
		MULTI_SERIALISATION_INTERFACE(Base);

		/// adds child effect and performs all initialisation
		template<typename Ty>
		Ty* addEffect(Ty* _new, bool activated = true);

		/// adds child from definition placed in file
		template<typename Ty>
		Ty* addEffect(const char* path);
		template<typename Ty>
		Ty* addEffect(std::istream& file, Res::DataScriptLoader& loader);

		

		/// returns first found efect of type T
		/// if there is no such the one returns nullptr
		/// @Warring make sure T derrives from Base otherwise something might go wrong
		template<typename T> shared_ptr<T> getEffect();

	private:
		vector<unique_ptr<Base>> childs;
	
	public:
		Game::Actor*		getActor()		{ assert(actor); return actor; }
		const Game::Actor*	getActor() const{ assert(actor); return actor; }
		void setActor(Game::Actor* s)
		{ 
			actor = s;  
			for (auto it = childs.begin(); it != childs.end(); ++it)
				it->get()->setActor(s);
		}
	
		Base*		getParent()			{ return parent; }
		const Base*	getParent() const	{ return parent; }
		void setParent(Base* s)			{ parent = s; }


		/// changes effects activate state
		void setActivated(bool b);
		bool isActivated() const			{ return bActivated; }

		void markAsReadyToRemove() { bIsReadyToRemove = true; }

	private:
		/// owner is the actor which holds effect and is affected by it
		Game::Actor* actor;
		/// effect this one is connected to
		/// nullptr means this is root component (if owner is set up)
		Base* parent;

		/// whether or not the efect is activated
		/// if not effect does not get updated nor take collision events
		bool bActivated :1;
		/// tells whether actor should be removed in the next frame
		bool bIsReadyToRemove :1;

	protected:
		/// serialisation functions - to allow effects to be sierialised from/into .res files
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver)  const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};
	
	template<typename Ty>
	Ty* Base::addEffect(Ty * _new, bool activated)
	{
		assert(_new);
		childs.push_back(unique_ptr<Base>(_new));
		
		_new->setActor(getActor());
		_new->setParent(this);

		_new->onInit();
		_new->setActivated(activated);

		return _new;
	}

	template<typename Ty>
	Ty * Base::addEffect(const char * path)
	{
		std::ifstream file;
		file.open(path);

		if (file)
		{
			DataScriptLoader loader;
			loader.nextLine(file);
			Ty* _new = addEffect<Ty>(file, loader);
			file.close();
			return _new;
		}

		// log error
		std::cout << "Falied to open the file: \"" << path << "\" to load\n";
		std::cout << "badbit " << file.badbit << ", failbit" << file.failbit << ", eof" << file.eofbit;
		return nullptr;
	}
	template<typename Ty>
	Ty * Base::addEffect(std::istream& file, Res::DataScriptLoader& loader)
	{
		Base* _new = new Ty();
		addEffect(_new)->deserialise(file, loader);
		return _new;
	}

	template<typename T>
	shared_ptr<T> Base::getEffect()
	{
		T* ptr;
		for (auto it = childs.begin(); it != childs.end() ++it)
			if ((ptr = dynamic_cast<T>(**it)))
				return ptr;
		return nullptr;
	}
}