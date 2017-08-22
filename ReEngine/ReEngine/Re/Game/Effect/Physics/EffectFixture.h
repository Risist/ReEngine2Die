#pragma once

#include <Re/Game/Effect/Physics/EffectRigidbody.h>

namespace Effect
{
	/// fixture effects holds ptr to box2D fixture
	/// receives from it event about collisions
	/// and propagates to his childrens
	///
	/// @note only Rigidbody can be parent for Fixture
	/// @note collision events are called only for childs of rigidbody and fixtures
	class Fixture final : public Composite
	{
		SERIALISATION_NAME(Fixture)
	public:

		/// default constructor -> does not create fixture
		Fixture();

		/// creates fixture of type @type with propertites from resource manager
		/// @param:fixtureDefId		id of fixture def in fixtureDefInst
		/// @param:shapeId			id of shape from circleShapeInst/polygonShapeInst/chainShapeInst
		/// @param:type				type of shape to detect from which container get shape
		///						only available polygon,circle and chain, do not use others
		Fixture(ResId fixtureDefId, ResId shapeId, b2Shape::Type type);

		/// creates fixture from manually provided definition
		Fixture(const b2FixtureDef* def);

		virtual ~Fixture();

	public:

		Fixture* createFixture(ResId fixtureDefId, ResId shapeId, b2Shape::Type type);
		Fixture* createFixture(const b2FixtureDef* def);

		void destroyFixture();

		b2Fixture* getFixture() const { return fixture; }

	public: /// events

		virtual bool canBeParent(Base* potentialParent) const override { return dynamic_cast<Rigidbody*>(potentialParent); }

		virtual void onDeconstructionFixture(b2Fixture* fixture) override;

	private:
		b2Fixture* fixture;

		ResId fixtureDefId{0};
		ResId shapeId{0};
	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader)override;
	};
}