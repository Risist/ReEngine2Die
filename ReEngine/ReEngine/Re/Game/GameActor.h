#pragma once
#include <Re\Game\Effect\EffectBase.h>
#include <Re\Game\Effect\EffectTransformable.h>

namespace Game
{

	/// well... it seems that actor is no longer needed

	/// responsibilities of an actor:
	///		Physics : DONE(rigidbody), raycasting
	///		DONE(Transformable)
	///		Input handling - InputController class 
	///		Instigator - actor responsible for actions of the actor
	///		DONE(alive/death managing)
	///		frame updating to alive state


	class Actor : public Effect::Transformable
	{
	public:
		MULTI_SERIALISATION_INTERFACE(Actor);


		Actor();
		virtual ~Actor();

		/// name of the actor, a way to identify it in runtime
		std::string name;

		bool isAlive() const { return bAlive; }
		void setAlive(bool s = true) { bAlive = s; }

		/// function should be called every frame
		/// chooses right form of updating object state
		///	@param:dt		delta of time elapsed between frames
		bool onFrame(sf::Time dt);

		/// an override to turn off/on rigidbody
		void onSetActivated() override;


		/// an actor mentally responsible on this one, 
		Actor* instigator{ nullptr };

	private:
		
		/// whether or not to update normally.
		/// if equal to true onUpdate event is called every frame
		/// otherwise onDeath, which gives possiblility of destroing actor
		bool bAlive : 1;

	public:

		b2Body& getRigidbody() const { assert(body);  return *body; };
		bool isRigidbodyCreated() const { return body; }
		/// creates rigidbody of the actor ( for more info look into box2d documentation)
		void createRigidbody(const b2BodyDef& def);
		void destroyRigidbody();

	private: // phisics
		/// physics rigidbody controlled by the actor
		b2Body* body{nullptr};

	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};


	/**
	/// Base class for all objects within the scene
	/// Has many events what can be overrided;
	/// Create actors only by createActor function of Game::World
	class Actor : public sf::Transformable, public Efect::Multi
	{
	public:
		Actor();
		virtual ~Actor();

		//// Events

		/// in onInit initiate all heavy parametrs
		/// like models, colliders ect
		/// that are constant to every Actor of the class
		virtual void onInit() override;

		/// event called every frame
		/// dt is the time elapsed from last update
		virtual void onUpdate(sf::Time dt) override;

		/// check whether or not the Actor is still alive
		/// used mostly by Game::World to decide which one 
		/// from onDeath and onUpdate should be called
		bool isAlive() const
		{
			return !readyToRemove;
		}
		void setAlive(bool s)
		{
			readyToRemove = !s;
		}
		/// update-like event called after death of an object
		/// di is the time elapsed from last update
		/// returns whether the object can be destroyed
		virtual bool onDeath(sf::Time dt) override;


		/// When the object collides with other those events are send
		/// taken actor is the other one with collision appeared
		/// contact is all collision data sended by box2d ( for more info look into box2d documentation )
		/// events wont be called until the Actor hasn't rigidbody created ( use createRigidbody function)
		virtual void onCollisionEnter(Actor& otherActor, b2Contact& contact) override;
		virtual void onCollisionExit(Actor& otherActor, b2Contact& contact) override;

		/// define custom filtering to the object
		virtual bool shouldCollide(b2Fixture* myFixture, b2Fixture* otherFixture) override;

	public:
		/// creates rigidbody of the actor ( for more info look into box2d documentation)
		void createRigidbody(const b2BodyDef& def);
		/// returns rigidbody assigned to the Actor
		/// @Warring call this function only if you are sure body has been created
		b2Body& getRigidbody() { assert(isRigidbodyCreated()); return *rigidbody; }
		const b2Body& getRigidbody() const { assert(isRigidbodyCreated()); return *rigidbody; }
		bool isRigidbodyCreated() const { return rigidbody != nullptr; }
		void destroyRigidbody();

	public:

		/// update transform of rigidbody to fit into Actors transform
		/// @Warring call this only when rigidbody is created
		void updateRigidbody(const Vector2D& originOffset = Vector2D(0, 0));

		/// update transform of Actor to fit into rigidbodys transform
		/// @Warring call this only when rigidbody is created
		void updateTransform(const Vector2D& originOffset = Vector2D(0, 0));

		/// updates taken as an argument transformable object to fit into actor's transform
		void updateGraphics(Transformable& transformable);
		void updateGraphics(Graphics::ModelDef& modelDef);

	public:
		/// overrided operators
		__forceinline Vector2D getPosition() const { return Transformable::getPosition(); }
		__forceinline Angle getRotation() const { return Degree(Transformable::getRotation()); }
		__forceinline Vector2D getScale() const { return Transformable::getScale(); }

		void setActive(bool s);
		bool isActive() { return activated; }

	protected:

		friend class World;

		/// rigidbody used by the actor. To get any collision events you need to have it filled;
		b2Body *rigidbody;

	private:
		/// actor has no owner ( in terms of efect)
		using Efect::Multi::setOwner;
		using Efect::Multi::getOwner;

		/// use set/is Active instead;
		using Efect::Base::activated;
	};*/

}