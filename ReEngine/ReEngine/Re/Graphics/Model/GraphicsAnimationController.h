#pragma once
#include <Re\Graphics\Model\GraphicsAnimationPart.h>

namespace Graphics
{

	class Model;

	class AnimationController : public Res::ISerialisable
	{
	public:
		AnimationController();
		AnimationController(const char* path);
		AnimationController(std::istream& file, Res::DataScriptLoader& loader) { deserialise(file, loader); };

		/// call this on every frame before model update/ after drawing
		/// updates model's actulDef to fullfil model state after appling animation
		void onUpdateModel();

		/// update animation to play forward/backward
		/// if exceeds [stepMin, stepMax] range( inclusive) 
		/// then animation is aligned to the edge step value
		/// and stop played
		/// @return did the animation have exceed [stepMin,stepMax] range?
		__forceinline bool updateInRange()
		{
			return updateInRange(speed);
		}
		/// version which uses speed taken as an argument
		/// instead speed holded in the class
		bool updateInRange(float32 speed);
		
		/// updates animation to play forward/backward
		/// if exceeds [stepMin, stepMax] range( inclusive) 
		/// speed changes its sign and animation is played in other direction
		/// @return did the animation have exceed [stepMin,stepMax] range?
		__forceinline bool updateReturn()
		{
			return updateReturn(speed);
		}
		/// version which uses speed taken as an argument
		/// instead speed holded in the class
		/// @Note - speed value can change its sign in order of calling the function
		bool updateReturn(float32& speed);
		
		__forceinline bool updateRestart(Step_t restartStep = 0) { return updateRestart(speed, restartStep);  }
		bool updateRestart(float32 speed, Step_t restartStep);


		__forceinline bool updateTowards(Step_t towards) { return updateTowards(speed, towards);  }
		bool updateTowards(float32 speed, Step_t towards);

		/// add new animation part 
		/// this should be created by new keyword because of future deletion
		void addPart(const AnimationPart& newPart);

		/// attach the animation to model
		/// animation parts holds ptr's to actualDef from models part 
		/// modelId determine which modelPart is choosen
		/// call after initialisation of modelId and before first onUpdate
		/// @Warring! slow operation. Don't use that on every frame 
		/// @Warring! make sure model after rewrite to vector will have enough elements
		///				that there is no part with modelId not pointing to model
		void attachToModel(Model& model);
		void attachToModel(vector<Model*>& model);

		/// adds to animation steps which will redo the animation between min and max step 
		/// to the state animation will be in initial step
		/// version for forward step playing
		void insertBackSteps(Step_t min, Step_t max, Step_t initial = 0);
		
		/// set step to the new value
		void reset(Step_t _new = 0);

		bool isStepInRange(Step_t min, Step_t max) const
		{
			return step >= min && step <= max;
		}
		Step_t getStep() const { return step; }
		Step_t getLenght() const { return stepMax - stepMin; }

		/// animation is only playable within the range of [stepMin, stepMax] inclusive
		Step_t stepMin{ 0 };
		Step_t stepMax{ 100.f };
	
		/// speed with the animation will be played
		/// if speed is positive the animation will be played forward
		/// otherwise backward
		Step_t speed{0};
	private:
		/// actual speed of animation
		Step_t step{0};

		
		/// animation parts holded
		vector<AnimationPart> parts;

	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};


}