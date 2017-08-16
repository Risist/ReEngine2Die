#include <Re\Game\GameWorld.h>
#include <Re\Graphics\Camera.h>
extern RenderWindow wnd;

extern thor::ActionMap<string> actionMap;

namespace Game
{
	World world;

	World::World()
		: physicsWorld(Vector2D())
	{
		// listeners
		physicsWorld.SetContactListener(this);
		physicsWorld.SetContactFilter(this);
	}

	void World::clear()
	{
		actors.clear();
	}

	
	void World::onFrame(sf::Time dt)
	{
		/// fixed update (?)
		physicsWorld.Step(1.f/60.f, velocityIterations, positionIterations);


		/// add newly spawned actors to not destroy iteration
		for (auto it = actorsToAdd.begin(); it != actorsToAdd.end();++it)
		{
			it->get()->onSpawn();
			Actor* ac = it->release();
			actors.push_back(unique_ptr<Actor>(ac));
		}
		actorsToAdd.clear();
		

		/// main actors update
		for (auto it = actors.begin(); it != actors.end();)
		{
			Actor* actorIt = it->get();
			
			if(actorIt->onFrame(dt))
			{
				actors.erase(it++);
			}
			else
			{
				++it;
			}
		}

		/// to show physics colliders
		if (actionMap.isActive("debugPhysics"))
			debugDisplayPhysics(Color(50, 50, 50, 200), Color(20, 20, 20, 200));
	}

	

	static struct RaycastCallback : public b2RayCastCallback
	{
		virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override
		{
			RaycastResult result = { 
				(Game::Actor*)fixture->GetBody()->GetUserData(),
				fixture,
				Vector2D(point)*toSfPosition,
				Vector2D(normal)*toSfPosition,
				fraction
			};
			return raycastCallback_function(result);
		}
		function<float32(const RaycastResult&)> raycastCallback_function;
	};
	static struct QueryCallback : public b2QueryCallback
	{
		virtual bool ReportFixture(b2Fixture* fixture) override { return queryCallback_function(fixture); }
		function<bool(b2Fixture*)> queryCallback_function;
	};

	void World::queryAABB(const Vector2D & loverBound, const Vector2D & upperBound, function<bool(b2Fixture*)> callback)
	{
		b2AABB aabb;
		aabb.lowerBound = b2Vec2(loverBound.x*toB2Position, loverBound.y*toB2Position);
		aabb.upperBound = b2Vec2(upperBound.x*toB2Position, upperBound.y*toB2Position);
		QueryCallback queryCallback;
		queryCallback.queryCallback_function = callback;
		physicsWorld.QueryAABB(&queryCallback, aabb);
	}
	void World::raycast(const Vector2D & p1, const Vector2D & p2, function<float32(const RaycastResult&)> callback)
	{
		RaycastCallback raycastCallback;
		raycastCallback.raycastCallback_function = callback;
		physicsWorld.RayCast(&raycastCallback, p1*toB2Position, p2*toB2Position);
	}


	void World::debugDisplayPhysics(Color clNotColliding, Color clColliding)
	{
		auto bodyIt = physicsWorld.GetBodyList();
		while (bodyIt)
		{
			if (!bodyIt->IsActive())
				continue;

			auto fixtureIt = bodyIt->GetFixtureList();
			while (fixtureIt)
			{
				auto _shape = fixtureIt->GetShape();
				
				switch (_shape->GetType())
				{
				case b2Shape::e_circle:
				{
					auto shape = (b2CircleShape*)_shape;
					sf::CircleShape sh;
					sh.setPosition((Vector2D)bodyIt->GetPosition() *toSfPosition 
						+ Vector2D(shape->m_p.x*toSfPosition, shape->m_p.y*toSfPosition).getRotated(Radian(bodyIt->GetAngle()) ));
					
					sh.setRadius(shape->m_radius *toSfPosition);

					sh.setRotation( Radian(bodyIt->GetAngle() ).asDegree() );
					//sh.setOrigin(
						//sh.getRadius() + shape->m_p.x*toSfPosition, 
						//sh.getRadius() + shape->m_p.y*toSfPosition);
					sh.setOrigin(
						sh.getRadius(),
						sh.getRadius());
					sh.setFillColor(clNotColliding);

					cam.draw(sh);
				}
					break;
				case b2Shape::e_polygon:
				{
					auto shape = (b2PolygonShape*)_shape;
					sf::ConvexShape sh;
					sh.setPosition((Vector2D)bodyIt->GetPosition() *toSfPosition 
						+ Vector2D(shape->m_centroid.x*toSfPosition, shape->m_centroid.y*toSfPosition).getRotated(Radian(bodyIt->GetAngle())));
					sh.setRotation(Radian(bodyIt->GetAngle()).asDegree());


					sh.setPointCount(((b2PolygonShape*)_shape)->m_count);
					for (int i = 0; i < sh.getPointCount(); ++i)
						sh.setPoint(i, (Vector2D)((b2PolygonShape*)_shape)->m_vertices[i]*toSfPosition);

					sh.setFillColor(clNotColliding);
					cam.draw(sh);
				}
					break;
				default:
					break;
				}

				fixtureIt = fixtureIt->GetNext();
			}


			bodyIt = bodyIt->GetNext();
		}

	}


	

	void World::BeginContact(b2Contact * contact)
	{
		if (contact->IsTouching())
		{
			Game::Actor * actorA = (Game::Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
			Game::Actor * actorB = (Game::Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

			/// make sure can cast to reference
			assert(actorA != nullptr);
			assert(actorB != nullptr);

			actorA->onCollisionEnter(*actorB, *contact);
			actorB->onCollisionEnter(*actorA, *contact);
		}
	}
	void World::EndContact(b2Contact * contact)
	{
		if (contact->IsTouching())
		{
			Game::Actor * actorA = (Game::Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
			Game::Actor * actorB = (Game::Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

			assert(actorA != nullptr);
			assert(actorB != nullptr);
			
			actorA->onCollisionExit(*actorB, *contact);
			actorB->onCollisionExit(*actorA, *contact);
		}
	}

	void World::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
	{
		Game::Actor * actorA = (Game::Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
		Game::Actor * actorB = (Game::Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

		assert(actorA != nullptr);
		assert(actorB != nullptr);

		actorA->onPostSolve(*actorB, *contact, *impulse);
		actorB->onPostSolve(*actorA, *contact, *impulse);
	}

	bool World::ShouldCollide(b2Fixture * fixtureA, b2Fixture * fixtureB)
	{
		assert(fixtureA->GetBody()->GetUserData() != nullptr);
		assert(fixtureB->GetBody()->GetUserData() != nullptr);

		return ((Actor*)fixtureA->GetBody()->GetUserData())->shouldCollide(fixtureA, fixtureB) 
			&& ((Actor*)fixtureB->GetBody()->GetUserData())->shouldCollide(fixtureB, fixtureA);
	}

	void World::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{

		saver.nextLine(file);
		auto it = actors.begin();
		do
		{
			saver.save<string>("actorType", it->get()->getName());
			it->get()->serialise(file, saver);
		}
		DATA_SCRIPT_MULTILINE_SAVE(file, saver, it != actors.end())
	}

	void World::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		DATA_SCRIPT_MULTILINE(file, loader)
		{
			string type = loader.load<string>("actorType", "Actor");
			Actor* ac = Actor::creationFunction(type.c_str());
			if (!ac)
				cerr << "wrong type of actor which is \"" << type << "\"" << endl;
			else
			{
				addActor(ac);
				ac->deserialise(file, loader);
			}
		}
	}


}