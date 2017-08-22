#include <Re\Game\Effect\Graphical\EffectDebugShapes.h>
#include <Re\Graphics\Camera.h>
#include <Re\Game\GameActor.h>

namespace Effect
{
namespace Debug
{ 
	GraphicsCircle::GraphicsCircle(Color fillColor, float32 radius)
	{
		shape.setRadius(radius);
		shape.setOrigin(radius, radius);
		shape.setFillColor(fillColor);
	}
	bool GraphicsCircle::canBeParent(Base * potentialParent) const
	{
		return dynamic_cast<Game::Actor*>(potentialParent);
	}
	void GraphicsCircle::onInit()
	{ 
		updateTarget = getActor();
	}
	void GraphicsCircle::onUpdate(sf::Time dt)
	{
		Super::onUpdate(dt);
		assert(updateTarget);
		
		shape.setPosition(updateTarget->getPosition() + getPosition().getRotated(updateTarget->getRotation()) );
		shape.setRotation(updateTarget->getRotation().asDegree() + getRotation().asDegree());

		cam.draw(shape); 
	}

	void GraphicsCircle::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		/// TODO serialisation
		Super::serialiseF(file, saver);
	}

	void GraphicsCircle::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		shape.setFillColor(Color(
			(sf::Uint8)loader.load<float32>("clR", 255.f),
			(sf::Uint8)loader.load<float32>("clG", 255.f),
			(sf::Uint8)loader.load<float32>("clB", 255.f),
			(sf::Uint8)loader.load<float32>("clA", 255.f)
		));
		setRadius(loader.load<float32>("radius", 100.f));
		ResId tsId = loader.load<ResId>("tsId", 0);
		if (tsId != 0)
			setTexture(tsId);
		Super::deserialiseF(file, loader);
	}


	/**
	GraphicsRect::GraphicsRect(const Vector2D & wh, Color fillColor, Angle rotation,
		const Vector2D & offset, UpdateMode _updateMode)
		:updateMode(_updateMode)
	{
		shape.setFillColor(fillColor);
		//shape.setOrigin(wh*0.5 + offset);
		shape.setRotation(rotation.asDegree());
		shape.setPosition(offset);

		shape.setPointCount(4);
		shape.setPoint(0, Vector2D(-wh.x * 0.5f, -wh.y * 0.5f));
		shape.setPoint(1, Vector2D(wh.x * 0.5f, -wh.y * 0.5f));
		shape.setPoint(2, Vector2D(wh.x * 0.5f, wh.y * 0.5f));
		shape.setPoint(3, Vector2D(-wh.x * 0.5f, wh.y * 0.5f));

	}
	void GraphicsRect::onUpdate(sf::Time dt)
	{
		//sf::ConvexShape sh = shape;
		getActor()->updateGraphics(shape);
		//sh.move(shape.getPosition() );
		//sh.rotate(shape.getRotation());
		cam.draw(shape);
		//cout << "rotation of " << getActor() << " is " << shape.getRotation() << endl;
	}*/
}
}