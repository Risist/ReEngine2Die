#include <Re\Graphics\Gui\GuiButton.h>
#include <Re\Graphics\ResourceManager.h>

extern RenderWindow wnd;

namespace Gui
{
	State::State(const sf::Color& _cl, ResId _tsId)
		:  cl(_cl),  tsId(_tsId), ts(tsInst[_tsId])
	{

	}

	State::State(ResId _tsId, const sf::Color & _cl)
		: cl(_cl), tsId(_tsId), ts(tsInst[_tsId])
	{
	}

	void State::deserialise_Index(const std::string & preName, std::istream & file, Res::DataScriptLoader & loader)
	{
		ResId tsId = loader.load<ResId>( preName + "Ts", (ResId)0);
		if (tsId) ts = tsInst[tsId];

		cl.r = loader.load(preName + "ClR", 255);
		cl.g = loader.load(preName + "ClG", 255);
		cl.b = loader.load(preName + "ClB", 255);
		cl.a = loader.load(preName + "ClA", 255);
	}


	Button::Button()
	{
	}
	void Button::onUpdate(sf::RenderTarget & target, sf::RenderStates states)
	{
		sh.setPosition(getActualPosition());

		bool mouseOn = isMouseOnWindow();

		if ((mouseKey.isReady() && mouseOn) || shortKey.isReady())
		{
			assert(eventOnPress);
			eventOnPress();

			sh.setFillColor(statePressed.cl);
			statePressed.ts.setRectShape(sh);
		}
		else if (mouseOn)
		{
			sh.setFillColor(stateMouseOn.cl);
			stateMouseOn.ts.setRectShape(sh);
		}
		else
		{
			sh.setFillColor(stateMouseOut.cl);
			stateMouseOut.ts.setRectShape(sh);
		}

		target.draw(sh, states);
	}

	bool Button::isMouseOnWindow() const
	{
		Vector2D mousePos = sf::Mouse::getPosition(wnd);
		return mousePos.x > getActualPosition().x - halfWh.x &&
			mousePos.x < getActualPosition().x + halfWh.x	&&
			mousePos.y > getActualPosition().y - halfWh.y	&&
			mousePos.y < getActualPosition().y + halfWh.y;
	}

	void Button::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		/// TODO
	}

	void Button::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		Base::deserialiseF(file, loader);
		stateMouseOn.deserialise_Index("mouseOn", file, loader);
		stateMouseOut.deserialise_Index("mouseOut", file, loader);
		statePressed.deserialise_Index("press", file, loader);
	}
}