#pragma once
#include <Re\Game\Effect\EffectTransformable.h>
#include <Re\Graphics\ResourceManager.h>

/*
This file contains sevral classes used mainly for debbuging purposes
They display some of sfml shapes, working in Effect hierarchy
*/
namespace Effect
{
namespace Debug
{ 
	class GraphicsCircle : public Transformable
	{
	public:
		GraphicsCircle( sf::Color fillColor = sf::Color::White, float32 radius = 100.f);

		virtual void onInit() override;
		virtual void onUpdate(sf::Time dt) override;

		GraphicsCircle* setColor(sf::Color cl)
		{
			shape.setFillColor(cl);
			return this;
		}
		GraphicsCircle* setRadius(float32 radius)
		{
			shape.setRadius(radius);
			shape.setOrigin({ radius,radius });
			return this;
		}
		GraphicsCircle* setTexture(const sf::Texture& ts)
		{
			shape.setTexture(&ts);
			return this;
		}
		GraphicsCircle* setTexture(ResId id)
		{
			tsInst[id].setCircleShape(shape);
			return this;
		}
		GraphicsCircle* setUpdateTarget(Transformable* s)
		{
			updateTarget = s;
			return this;
		}

		/// display data
		sf::CircleShape shape;
		Transformable* updateTarget;
	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

	/**
	class GraphicsRect : public Base
	{
	public:
		enum UpdateMode
		{
			none,
			toTransform
		}updateMode{ toTransform };

		GraphicsRect(const Vector2D& wh = Vector2D(), Color fillColor = Color::White, Angle rotation = Angle(),
			const Vector2D& offset = Vector2D(), UpdateMode updateMode = toTransform);

		virtual void onUpdate(sf::Time dt) override;


		/// display data
		sf::ConvexShape shape;

	};*/
}
}