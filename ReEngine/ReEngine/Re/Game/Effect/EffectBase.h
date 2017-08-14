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



	public:/// childs
		/// function creating effect depended on its name 
		/// for purpose of creating in scripts
		/// if you create your own gameplay-depended effects provide new creation function
		/// in which if creation by default version will fail (return nullptr)
		/// you will check whether you chave class appropriate for the name
		MULTI_SERIALISATION_INTERFACE(Base);

		/// adds child effect and performs all initialisation
		/// returns shared
		template<typename Ty>
		shared_ptr<Ty> addEffect(Ty* _new);
		

		/// returns first found efect of type T
		/// if there is no such the one returns nullptr
		/// @Warring make sure T derrives from Base otherwise something might goes wrong
		template<typename T> shared_ptr<T> getEffect();

	private:
		vector<shared_ptr<Base>> childs;
	
	public:
		weak_ptr<Game::Actor> getOwner()			{ assert(owner._Get() != nullptr); return owner; }
		const weak_ptr<Game::Actor> getOwner() const{ assert(owner._Get() != nullptr); return owner; }
		void setOwner(weak_ptr<Game::Actor> s)
		{ 
			owner = s;  
			for (auto it = childs.begin(); it != childs.end(); ++it)
				it->get()->setOwner(s);
		}
	
		weak_ptr<Base>			getParent()			{ return parent; }
		const weak_ptr<Base>	getParent() const	{ return parent; }
		void	setParent(weak_ptr<Base> s)			{ parent = s; onInit(); }


		/// changes effects activate state
		void setActivated(bool b);
		bool isActivated() const			{ return bActivated; }

		void markAsReadyToRemove() { bIsReadyToRemove = true; }

	private:
		/// owner is the actor which holds effect and is affected by it
		weak_ptr<Game::Actor> owner;
		/// effect this one is connected to
		/// nullptr means this is root component (if owner is set up)
		weak_ptr<Base> parent;
		/// to allow setting up parent ptr, we need to hold reference to myPtr 
		/// (shared should be probably located in Actor)
		weak_ptr<Base> myPtr;

		/// whether or not the efect is activated
		/// if not effect does not get updated nor take collision events
		bool bActivated :1;
		/// tells whether actor should be removed in the next frame
		bool bIsReadyToRemove :1;

	protected:
		/// serialisation functions - to allow effects to be sierialised from/into .res files
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver)  const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;


		friend class Actor;
	};
	
	template<typename Ty>
	shared_ptr<Ty> Base::addEffect(Ty * _new)
	{
		childs.push_back(std::make_shared<Base>(_new));
		_new->setParent(myPtr);
		_new->setParent(getParent());

		_new->myPtr = childs.back();
		_new->onInit();

		return _new->myPtr;
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