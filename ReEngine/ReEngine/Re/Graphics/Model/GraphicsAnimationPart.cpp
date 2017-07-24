#include <Re\Graphics\Model\GraphicsAnimationPart.h>
#include <Re\Common\Math\Math.h>

namespace Graphics
{
	AnimationPart::AnimationPart()
	{
	}
	AnimationPart::AnimationPart(size_t _modelId, const ModelDef& modelDef, Step_t _stepMin, Step_t _stepMax)
		: ModelDef(modelDef), stepMin(_stepMin), stepMax(_stepMax), modelId(_modelId)
	{
	}
	AnimationPart::AnimationPart(char * path)
	{
		deserialise(path);
	}
	void AnimationPart::onUpdateModel() const
	{
		assert(defToUpdate);
		countOffset(*defToUpdate);
	}
	void AnimationPart::countOffset(ModelDef & out) const
	{
		assert(step != nullptr);

		out.color += color *getActualStep();
		out.pos += pos *getActualStep();
		out.scale += scale*getActualStep();
		out.rotAround += rotAround * getActualStep();
		out.rot += rot * getActualStep();
		out.rotSprite += rotSprite * getActualStep();
		out.origin += origin *getActualStep();
	}
	bool AnimationPart::isWithinRange() const
	{
		return getStep() >= stepMin && getStep() <= stepMax;
	}

	Step_t AnimationPart::getActualStep() const
	{
		return clamp(getStep(), stepMin, stepMax) + stepOffset;
	}

	ModelDef AnimationPart::getDefAtStep(Step_t step) const
	{
		return (ModelDef)(*this) *  ( clamp(step, stepMin, stepMax) + stepOffset);
	}

	void AnimationPart::serialiseF(std::ostream & file, Res::DataScriptSaver & saver) const
	{
		/// serialization of animation available only in editor
#	ifdef RE_EDITOR
		ModelDef::serialiseF(file, saver);

		saver.save("model", modelId);
		saver.save("stepMin", stepMin);
		saver.save("stepMax", stepMax);
		saver.save("stepOffset", stepOffset);
#	endif // RE_EDITOR
	}
	void AnimationPart::deserialiseF(std::istream & file, Res::DataScriptLoader & loader)
	{
		//ModelDef::deserialiseF(file, loader);	
		
		pos.x = loader.load("posX", ModelDef::zero.pos.x);
		pos.y = loader.load("posY", ModelDef::zero.pos.y);

		origin.x = loader.load("originX", ModelDef::zero.origin.x);
		origin.y = loader.load("originY", ModelDef::zero.origin.y);

		scale.x = loader.load("scaleX", ModelDef::zero.scale.x);
		scale.y = loader.load("scaleY", ModelDef::zero.scale.y);

		rot = Degree(loader.load("rot", ModelDef::zero.rot.asDegree()));
		rotAround = Degree(loader.load("rotAround", ModelDef::zero.rotAround.asDegree()));
		rotSprite = Degree(loader.load("rotSprite", ModelDef::zero.rotSprite.asDegree()));

		color.r = loader.load("clR", ModelDef::zero.color.r);
		color.g = loader.load("clG", ModelDef::zero.color.g);
		color.b = loader.load("clB", ModelDef::zero.color.b);
		color.a = loader.load("clA", ModelDef::zero.color.a);

		/////
		modelId = loader.load("model", (size_t)0);

		stepMin = loader.load("stepMin", -numeric_limits<Step_t>::max() );
		stepMax = loader.load("stepMax", numeric_limits<Step_t>::max());
		stepOffset = loader.load("stepOffset", 0);
	}
}