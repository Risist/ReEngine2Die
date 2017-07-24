#include <Re\ReEngine.h>
#include <Re\Lambdas.h>
#include <Re\Game\Efect\efects.h>
#include "Layers.h"
#include <Re\Graphics\Graphics.h>

/// this file contains tests of how engine works
/// can be used as example of usage (but currently changes dynamically and is not comented enough )


Game::Actor* createBlood()
{
	auto actor = new Game::Actor;
	auto efModel = actor->addEfect(new Efect::Model("modelBlood.txt"));

	actor->addEfect(new Efect::RemoveAfterDelay(sf::seconds(30)));
	return actor;
}
Game::Actor* createTesterMovementBot(Game::Actor* player)
{ //
	Game::Actor *actor = (new Game::Actor);
	actor->setPosition(Vector2D(0, randRange(0.f, 500.f)).getRotated(randRange(Angle::zero, Angle::full)));
	actor->addEfect(new Efect::Rigidbody(25.f, 20.f));
	actor->addEfect(new Efect::ColliderCircle(20.f, 1.f));


	auto efMovement = actor->addEfect(new Efect::MovementAim(7.5,
		new Efect::RotateToDirection(Efect::RotateToDirection::smoothPhysics, 0.03f))
	);

	auto efSensor = actor->addEfect(new Efect::Sensor(sf::seconds(0.1f)))
		->addFilter( new Sense::FilterCircle( 1000.f ))
		;

	actor->addEfect(new Efect::UpdateTransform());
	auto efModel = actor->addEfect(new Efect::Model("model.txt"));
	
	auto efMovementFollow = actor->addEfect(new Efect::MovementFollow(player, efMovement))
		->setDistance(0, 200);
	
	actor->addEfect(new Efect::RandomMovement(efMovement, 5))
		->setRadius(50, 150)
		->setTime(seconds(0.5), seconds(1.95))
		->setOffset(Vector2D(0, 50));
	
	actor->addEfect(new Efect::Health());
	actor->addEfect(new Efect::SpawnOnDeath(createBlood) )->setLayer(Game::Layers::blood);

	return actor;
}
Game::Actor* createBullet()
{
	auto actor = new Game::Actor;

	actor->addEfect(new Efect::Rigidbody(25, 20));
	actor->addEfect(new Efect::ColliderCircle(20, 1));

	auto update = actor->addEfect(new Efect::UpdateTransform());
	auto efModel = actor->addEfect(new Efect::Model("modelBullet.txt"));

	actor->addEfect(new Efect::RemoveAfterDelay(seconds(1.5f)));
	actor->addEfect(new Efect::RemoveOnCollision );
	actor->addEfect(new Efect::Motor(15));

	actor->addEfect(new Efect::DamageOnCollision(50.f) );
	
	return actor;
}
Game::Actor* createPlayer()
{
	Game::Actor *actor = new Game::Actor;

	actor->addEfect(new Efect::Health());
	actor->addEfect(new Efect::SpawnOnDeath(createBlood) )->setLayer(Game::Layers::bullet);


	actor->addEfect(new Efect::Rigidbody(5, 20));
	auto f = actor->addEfect(new Efect::ColliderCircle(25, 1))->fixture;

	/**/
	auto efMovement = actor->addEfect(new Efect::RadiusMovement(10.f,
		new Efect::RotateToDirection(Efect::RotateToDirection::smoothPhysics, 0.03),
		40, 100, 300
	));/**/
	   /*auto efMovement = actor->addEfect(new Efect::MouseMovement(2.5,
	   new Efect::RotateToDirection(Efect::RotateToDirection::smoothPhysics, 0.03))
	   );*/


	actor->addEfect(new Efect::UpdateTransform());
	actor->addEfect(new Efect::GraphicsCircle(Color(150, 150, 150, 100), efMovement->rotationRadius));
	actor->addEfect(new Efect::GraphicsCircle(Color(150, 150, 150, 100), efMovement->movementRadius));
	auto efModel = actor->addEfect(new Efect::Model("model.txt"));

	actor->addEfect(new Efect::FollowCamera(Efect::FollowCamera::positionOnlySmooth, 0))->lerpAlpha = Vector2D(0.15, 0.5);


	actor->addEfect(new Efect::Animation("animation.txt", efModel->model))->eventControllAnimation = [=](Graphics::AnimationController& c)
	{
		if (efMovement->lastSpeedModificator > 0.25)
			c.updateInRange(c.speed * (efMovement->lastSpeedModificator + 0.25));
		else
			c.updateInRange(-c.speed * (1 - efMovement->lastSpeedModificator));
	};

	//actor->addEfect(new Efect::Throw(createBullet) )
	//	->setOffset(Vector2D(0, -100), Angle())
	//	->setLayer(Game::Layers::bullet);

	return actor;
}
Game::Actor* createObstacle()
{
	Game::Actor *actor = new Game::Actor;

	actor->setPosition(randRange(-500, 500), randRange(-500, 500));
	actor->setRotation(randRange(0, 360));
	actor->addEfect(new Efect::Rigidbody(25, 10));
	actor->addEfect(new Efect::ColliderBox(Vector2D(25, 25), 10));


	actor->addEfect(new Efect::UpdateTransform());
	actor->addEfect(new Efect::GraphicsRect(Vector2D(50, 50), Color(100, 100, 100, 150)));

	return actor;
}


void initKeys()
{
	actionMap["up"] = thor::Action(sf::Keyboard::W);
	actionMap["down"] = thor::Action(sf::Keyboard::S);
	actionMap["left"] = thor::Action(sf::Keyboard::A);
	actionMap["right"] = thor::Action(sf::Keyboard::D);

	actionMap["up2"] = thor::Action(sf::Keyboard::Up);
	actionMap["down2"] = thor::Action(sf::Keyboard::Down);
	actionMap["left2"] = thor::Action(sf::Keyboard::Left);
	actionMap["right2"] = thor::Action(sf::Keyboard::Right);


	actionMap["fire1"] = thor::Action(sf::Mouse::Left) || thor::Action(sf::Mouse::Right);
	actionMap["fire2"] = thor::Action(sf::Keyboard::Q, thor::Action::PressOnce);
	actionMap["fire3"] = thor::Action(sf::Keyboard::E, thor::Action::PressOnce);

	actionMap["debugPhysics"] = thor::Action(sf::Keyboard::Z, thor::Action::Hold);

}
Game::State* createState()
{
	return new Game::StateLambda(
		[]() /// init
	{
		//Game::world.addActor(new Game::Actor)->addEfect(new Efect::Sound(10))->setWhenPlaying([=]() {return actionMap["up"]; });
	},

		[](sf::Time dt)->Game::State*
	{
		Game::world.onUpdate(dt);
		cam.display(wnd);
		if (actionMap.isActive("restart"))
			return createState();
		return nullptr;
	}
	);

}
//Graphics::ModelPolygon model;

void init()
{
	srand((unsigned int)time(nullptr));
	wnd.create(VideoMode(800, 600), "ReEngine");
	wnd.setVerticalSyncEnabled(true);

	cam.create(Vector2D(wnd.getSize().x, wnd.getSize().y));
	cam.setBackgroundColor(Color(0, 0, 0));
	cam.setDarkness(1);
	cam.setBackgroundColor(Color(200, 200, 200));

	res.deserialise("Resources.txt");

	initKeys();


}


void update()
{	
	static Clock performanceClock;
	performanceClock.restart();
	cam.display(wnd);

	sf::Text txt;
	{
		static int32 average = performanceClock.getElapsedTime().asMilliseconds();
		average = (average * 49 + performanceClock.getElapsedTime().asMilliseconds()) /50;

		txt.setPosition(0, 0);
		txt.setFont(res.fonts[1]);
		txt.setCharacterSize(35);
		txt.setFillColor(Color::White);
		txt.setStyle(sf::Text::Bold);
		std::stringstream ss;
		ss << "ms: " << average;
		txt.setString(ss.str());
	}
	wnd.draw(txt);
}
void out()
{

}


int main()
{
	init();

	while (wnd.isOpen())
	{
		actionMap.clearEvents();
		Event ev;
		while (wnd.pollEvent(ev))
		{
			if (ev.type == Event::Closed
				|| (ev.type == Event::KeyPressed && ev.key.code == sf::Keyboard::F1))
				wnd.close();
			actionMap.pushEvent(ev);
		}
		update();

		wnd.display();
	}
	out();
}