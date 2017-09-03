#pragma once
#include <Re\Graphics\Gui\GuiBase.h>
#include <Re\Graphics\ResourceManager.h>

namespace Gui
{
	/// Data depended on button state packed in struct
	struct State
	{
		State(const sf::Color& _cl = Color::White, ResId tsId = 0);
		State(ResId tsId, const sf::Color& _cl);
		void setTexture(ResId id)
		{
			tsId = id;
			ts = tsInst[id];
		}
		sf::Color cl;
		ResId tsId;
		ResourceManager::TextureInstance ts;

		void deserialise_Index(const std::string& preName, std::istream& file, Res::DataScriptLoader& loader);
	};
	class Button : public Base
	{
		SERIALISATION_NAME(Button)
	public:
		Button();


	public: ////// events
		virtual void onUpdate(sf::RenderTarget& target, sf::RenderStates states) override;


	public: ////// setters
		Button* setGlobalState(const sf::Color& _cl = Color::White, ResId tsId = 0)
		{
			stateMouseOn = stateMouseOut = statePressed = State(_cl, tsId);
			return this;
		}
		Button* setStateMouse(const sf::Color& _cl = Color::White, ResId tsId = 0)
		{
			stateMouseOn = stateMouseOut = State(_cl,tsId);
			return this;
		}
		Button* setStateMouseOn(const sf::Color& _cl = Color::White, ResId tsId = 0)
		{
			stateMouseOn = State(_cl, tsId);
			return this;
		}
		Button* setStateMouseOut(const sf::Color& _cl = Color::White, ResId tsId = 0)
		{
			stateMouseOut = State(_cl, tsId);
			return this;
		}
		Button* setStatePressed(const sf::Color& _cl = Color::White, ResId tsId = 0)
		{
			statePressed = State(_cl, tsId);
			return this;
		}
		Button* setPressEvent(function<void()> ev)
		{
			eventOnPress = ev;
			return this;
		}
		Button* setWh(const Vector2f& wh)
		{
			Super::setWh(wh);
			sh.setSize(wh);
			sh.setOrigin(halfWh);
		}
		REDEFINE_SETTER_1(Button, setPosition, const Vector2f&);
		REDEFINE_SETTER_1(Button, setActivated, bool);

		Button* setMouseKey(sf::Mouse::Button key)
		{
			mouseKey.setKeyCode(key);
			mouseKey.desiredState = Control::Key::EPressState::pressedOnce;
			return this;
		}
		Button* addShortKey(sf::Mouse::Button key)
		{
			shortKey.addKey(Control::Key(key, Control::Key::EPressState::pressedOnce));
			return this;
		}
		Button* addShortKey(sf::Keyboard::Key key)
		{
			shortKey.addKey(Control::Key(key, Control::Key::EPressState::pressedOnce));
			return this;
		}

		
		
	protected:
		/// mouse key activates the button only if mouse is at button
		bool isMouseOnWindow() const;
		Control::Key mouseKey;

		/// short key, activates button anytime
		Control::MultiKey shortKey;
	
		/// Color & texture for each state:
		State stateMouseOn; ///<	mouse is on the button
		State statePressed; ///<	button is pressed
		State stateMouseOut;///<	mouse is not on the button

		/// what happeneds when button is pressed?
		function<void()> eventOnPress{ []() {} };

		/// rendering shape
		sf::RectangleShape sh;
	
	protected:
		/// Graphical propertites saved in files 
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

}