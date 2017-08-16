#pragma once
//#include <Re\Common\utility.h>
#include <Re\Game\GameActor.h>

namespace Game
{

	struct RaycastResult
	{
		Actor* actor;			/// actor responsible for hitted fixture
		b2Fixture* fixture;		/// fixture on the path of the ray
		Vector2D point;			/// point on which collision with ray occured
		Vector2D normal;		/// normal vector to the surface of hitted fixture
		float32 fraction;		/// fraction part - how far from the beggining of the ray, in range [0,1]
	};

	class World : 
		public Res::ISerialisable,
		private b2ContactFilter, 
		private b2ContactListener
	{
	public:
		World();
		
		/// @summary updates physics and all actors
		/// should be called every frame
		/// @param:dt		time elapsed between frames
		void onFrame(sf::Time dt);

		/// acquire a new actor into actors list
		/// layer specify order of running functions. if equal nullptr adds to actorList
		//template<class Act >
		//Act* addActor(Act *_new);
		Actor* addActor(Actor* _new);

	public:
		void debugDisplayPhysics(Color clNotColliding, Color clColliding);
		
		/// removes all actors from the world
		void clear();

		///
		void queryAABB(const Vector2D& loverBound, const Vector2D& upperBound, function<bool(b2Fixture*)> callback);
		void raycast(const Vector2D& p1, const Vector2D& p2, function<float32(const RaycastResult&)> callback);
		

		vector<unique_ptr<Actor>> actors;
		b2World physicsWorld;
	private:
		vector<unique_ptr<Actor>> actorsToAdd;
	
	private: // box2d callbacks
		/// @Note serialisation works only with actors 
		///			physics is not saved (even only by settings of efects)
		virtual void BeginContact(b2Contact* contact) override;
		virtual void EndContact(b2Contact* contact) override;
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	
		virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;

		

		
	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver)const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};
	extern World world;

	//template<class Act>
	//Act * World::addActor(Act * _new)
	inline Actor* World::addActor(Actor* _new)
	{
		assert(_new);

		actorsToAdd.push_back(unique_ptr<Actor>(_new) );
		_new->onInit();

		return (Actor*)_new;
	}
}