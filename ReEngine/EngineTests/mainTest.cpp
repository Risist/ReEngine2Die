#include <Re\ReEngine.h>


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


void init()
{
	srand((unsigned int)time(nullptr));
	wnd.create(VideoMode(800, 600), "ReEngine");
	wnd.setVerticalSyncEnabled(true);

	cam.create(Vector2D(wnd.getSize().x, wnd.getSize().y));
	cam.setBackgroundColor(Color(0, 0, 0));
	cam.setDarkness(1);
	cam.setBackgroundColor(Color(200, 200, 200));

	//res.deserialise("Resources.txt");
	//initKeys();

	Game::world.deserialise("world_test.txt");
}


void update()
{	
	static Clock performanceClock;
	performanceClock.restart();
	Game::world.onFrame(sf::seconds(1));
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