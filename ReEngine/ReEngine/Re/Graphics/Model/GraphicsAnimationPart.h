#pragma once
#include <Re\Graphics\Model\GraphicsModelDef.h>
#include <limits>

namespace Graphics
{
	/// type of step used at animation
	typedef float32 Step_t;

	/// Class describes animation part
	/// inherits from Transformable and its values shows how model part will change
	class AnimationPart : public ModelDef
	{
	public:
		/// default constructor
		AnimationPart();
		/// manually creates animation part
		AnimationPart(size_t modelId, const ModelDef& def, Step_t stepMin, Step_t stepMax );
		/// loades animation part from file
		AnimationPart(char* path);
		/// deserialise from loader
		AnimationPart(std::istream& file, Res::DataScriptLoader& loader) { deserialise(file, loader); }

		/// updates actual definition of model the animationPart is attached to
		/// should be called every frame before model update
		/// do not call this when the model is removed
		void onUpdateModel() const;

		/// count offset described by this animationPart after applying step 
		/// @Warring firstly step should be assigned 
		/// @param [out] Transformable& out 
		///		where to save counted transformable 
		void countOffset( ModelDef& out) const;

		/// determines whether or not animation is in right state for this part
		/// [stepMin, stepMax] inclusive
		bool isWithinRange() const;

		/// getStep() function returns step of whole animation
		/// this one returns step of part
		Step_t getActualStep() const;

		/// set pointner to new step
		/// should be done before any other operation is done
		/// otherwise class will assert
		/// AnimationController does that for you if step is created by controller
		void setStepPtr(Step_t *newStep)
		{
			step = newStep;
		}
		/// get value of actual step
		/// if step is not assigned will assert
		Step_t getStep() const
		{
			assert(step != nullptr);
			return *step;
		}
		ModelDef getDefAtStep(Step_t step) const;


		// index of model in revrited vector that the animation affects 
		size_t modelId{0};

		/// animation will be played only within those steps
		Step_t stepMin{ -numeric_limits<Step_t>::infinity() };
		Step_t stepMax{ numeric_limits<Step_t>::infinity() };
		/// addictionally added value to actual step non depended on animations nor min/max
		Step_t stepOffset{0};
		ModelDef* defToUpdate{nullptr};
	private:
		Step_t *step{nullptr};

	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

}