#include <Re\ReEngine.h>


class ef : public Effect::Base
{
public:

	Control::Timer timer = Control::Timer(sf::seconds(1));

	bool canBeParent(Effect::Base* p) const { return true; }

	void onUpdate(sf::Time dt) override
	{
		if (timer.isReadyRestart() )
		{
			cout << "Interia = " << getActor()->getRigidbody()->GetInertia() << endl;
			cout << "Mass = " << getActor()->getRigidbody()->GetMass() << endl;
			cout << endl;
		}

		//getActor()->getRigidbody()->ApplyTorque(1, true);
	}
};

class State : public Game::State
{
public:

	Control::Key restartKey;
	Graphics::Background background;
	
	Game::Actor *myActor;
	Effect::MovementAim* movement;

	virtual void onStart()
	{
		Super::onStart();
		cam.setScale(2.25f);
		res.deserialise("Resources.txt");

		{
			sf::Sprite sp;
			tsInst[100].setSprite(sp);
			sp.setScale(5, 5);
			sp.setOrigin(sp.getTextureRect().height*0.5f, sp.getTextureRect().width * 0.5f );
			background.addSprite(sp);
		}
		
		myActor = Game::world.createActor();
		myActor->setPosition({ 0,0 });
		myActor->createPlayerInput()
			->setInputKey("up", new Control::MultiKey(sf::Keyboard::W))
			->setInputKey("down", new Control::MultiKey(sf::Keyboard::S))
			->setInputKey("left", new Control::MultiKey(sf::Keyboard::A))
			->setInputKey("right", new Control::MultiKey(sf::Keyboard::D))

			->setInputKey("press", new Control::MultiKey(sf::Mouse::Left))
			
			->setInputAxis("xMove", "right", "left"	)
			->setInputAxis("yMove", "down",	 "up"	)
			;

		auto efModel = myActor->addEffect( new Effect::Model("model.txt"));
		myActor->addEffect(new Effect::AnimationManager(*efModel))
			->insertAnimation("anim_reaperPose.txt");

		auto efRigidbody = myActor->addEffect(new Effect::Rigidbody())
			->createRigidbody(Effect::RigidbodyDef()
				.setType(b2_dynamicBody)
				.setLinearDamping(10)
				.setAngularDamping(15)
			)
			->setSyncMode(Effect::Rigidbody::ETransformSyncMode::transformToRigidbody);

		efRigidbody->addEffect(new Effect::Fixture())
			->createFixture(Effect::CircleColliderDef()
				.setDensity(2.f)
				.setRadius(100.f)
			);


		//movement = efRigidbody->addEffect(new Effect::MouseMovement(2.5f));
		movement = efRigidbody->addEffect(new Effect::StaticMovement(2.5f))->setMinimalDistance(0.f);
		
		myActor->addEffect(new Effect::FollowCamera())->setSmooth(0.1f, 0.0f);

		restartKey.setKeyCode(sf::Keyboard::P);
		restartKey.desiredState = Control::Key::EPressState::pressedOnce;
	}

	virtual State* onUpdate(sf::Time dt = sf::seconds(1))
	{
		cam.draw(background);
		Game::world.onFrame(dt);
		cam.display(wnd);

		//cout << "isMoving = " << movement->isMoving() << endl;

		if(restartKey.isReady())
			return new ::State;
		return nullptr;
	}
	void onExit()
	{
		Game::world.clear();
		Gui::gui.clear();
	}

	
};


void init()
{
	srand((unsigned int)time(nullptr));
	wnd.create(VideoMode(800, 600), "ReEngine");
	wnd.setVerticalSyncEnabled(true);

	cam.create(Vector2D((float32)wnd.getSize().x, (float32)wnd.getSize().y));
	cam.setBackgroundColor(Color(0, 0, 0));
	cam.setDarkness(1);
	cam.setBackgroundColor(Color(200, 200, 200));

	//res.deserialise("Resources.txt");
	Game::stateManager.setState(new State);
}


void update()
{	
	static Clock performanceClock;
	performanceClock.restart();
	Game::stateManager.onUpdate();

	
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
				|| (ev.type == Event::KeyPressed && ev.key.code == sf::Keyboard::Escape))
				wnd.close();
			actionMap.pushEvent(ev);
		}
		update();

		wnd.display();
	}
	out();
}