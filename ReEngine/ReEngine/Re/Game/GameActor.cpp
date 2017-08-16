#include <Re\Game\GameActor.h>
#include <Re\Game\GameWorld.h>
#include <Re\Graphics\Model\GraphicsModelDef.h>

namespace Game
{

	// define function which will be used for serialisation of all standard actors
	MULTI_SERIALISATION_INTERFACE_IMPL(Actor)
	{
		MULTI_SERIALISATION_INTERFACE_CHECK(Actor);
		
		return nullptr;
	}

	Actor::Actor()
		: bAlive(true)
	{
		setOwner(this);
	}

	Actor::~Actor()
	{
		if (isRigidbodyCreated())
			destroyRigidbody();
	}

	bool Actor::onFrame(sf::Time dt)
	{
		/// dummy code, there is no pause code yet
		bool isGamePaused = false;

		if (isGamePaused)
			onPause(dt);
		else if (isAlive())
			onUpdate(dt);
		else
			return onDeath(dt);

		return false;
	}

	void Actor::onSetActivated()
	{
		if (isRigidbodyCreated())
			body->SetActive(isActivated());
		Transformable::onSetActivated();
	}

	void Actor::createRigidbody(const b2BodyDef & def)
	{
		/// TODO
		body = world.physicsWorld.CreateBody(&def);
		body->SetUserData(this);
	}

	void Actor::destroyRigidbody()
	{
		body->GetWorld()->DestroyBody(body);
		body->SetUserData(nullptr);
		body = nullptr;
	}

	void Actor::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		saver.save<string>	("name", name, "");
		saver.save<bool>	("alive", bAlive, true);

		Super::serialiseF(file, saver);
	}

	void Actor::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		name	= loader.load<string>	("name", "");
		bAlive	= loader.load<bool>		("alive", true);

		Super::deserialiseF(file, loader);
	}

}