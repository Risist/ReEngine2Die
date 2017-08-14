#pragma once

#include <Re\Game\Efect\EfectBase.h>

namespace Efect
{

	class ColliderCircle : public Base
	{
	public:
		ColliderCircle(float32 radius, float32 density = 1, Vector2D offset = Vector2D(),
			bool isSensor = false, float32 restitution = 0.3, float32 friction = 0.3);
		void onStart() override;

		ColliderCircle* setSensor(bool s)
		{
			fixtureDef.isSensor = s;
			return this;
		}
		ColliderCircle* setDensity(float32 s)
		{
			fixtureDef.density = s;
			return this;
		}
		ColliderCircle* setFriction(float32 s)
		{
			fixtureDef.friction = s;
			return this;
		}
		ColliderCircle* setOffset(Vector2D offset)
		{
			shapeDef.m_p = offset*toB2Position;
			return this;
		}
		ColliderCircle* setRadius(float32 s)
		{
			shapeDef.m_radius = s;
			return this;
		}
		ColliderCircle* setRestitution(float32 s)
		{
			fixtureDef.restitution = s;
			return this;
		}

		b2CircleShape shapeDef;
		b2FixtureDef fixtureDef;
		b2Fixture* fixture{nullptr};
	};

	class ColliderBox : public Base
	{
	public:
		/// wh - width and height
		ColliderBox(Vector2D wh = Vector2D(), float32 density = 1,
			bool isSensor = false, float32 restitution = 0.3, float32 friction = 0.3);
		
		void onStart() override;



		ColliderBox* setAsBox(const Vector2D& wh, const Vector2D& center = Vector2D(), Angle angle = Angle::zero)
		{
			shapeDef.SetAsBox(wh.x*toB2Position, wh.y*toB2Position, center*toB2Position, angle.asRadian());
			return this;
		}
		ColliderBox* addPoint(Vector2D position)
		{
			points.push_back(position);
			return this;
		}
		ColliderBox* setSensor(bool s)
		{
			fixtureDef.isSensor = s;
			return this;
		}
		ColliderBox* setDensity(float32 s)
		{
			fixtureDef.density = s;
			return this;
		}
		ColliderBox* setFriction(float32 s)
		{
			fixtureDef.friction = s;
			return this;
		}
		ColliderBox* setRestitution(float32 s)
		{
			fixtureDef.restitution = s;
			return this;
		}


		b2PolygonShape shapeDef;
		b2FixtureDef fixtureDef;
		b2Fixture* fixture{nullptr};
		vector<b2Vec2> points;
	};
}