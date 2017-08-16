#include <Re\ReEngine.h>

Control::Key restartKey;

class State : public Game::State
{
public:
	virtual void onStart()
	{
		Super::onStart();
		Game::world.deserialise("world_test.txt");

		restartKey.setKeyCode(sf::Keyboard::P);
		restartKey.desiredState = Control::Key::EPressState::pressedOnce;
	}

	virtual State* onUpdate(sf::Time dt = sf::seconds(1))
	{
		Game::world.onFrame(dt);
		cam.display(wnd);

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

	cam.create(Vector2D(wnd.getSize().x, wnd.getSize().y));
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