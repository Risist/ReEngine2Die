#include <Re\Game\Efect\Graphical\EfectModel.h>
#include <Re\Game\GameActor.h>
namespace Efect
{
	Model::Model(const char* path)
		:model(path)
	{
		model.rewriteToVectorRendering(modelsRendering);
		model.rewriteToVectorUpdate(modelsUpdate);
	}

	Model::Model(ResId ts)
		:model(ts)
	{
		model.rewriteToVectorRendering(modelsRendering);
		model.rewriteToVectorUpdate(modelsUpdate);
	}

	void Model::onInit()
	{
		setParent(getOwner());
	}

	void Efect::Model::onUpdate(sf::Time dt)
	{
		//model.drawRecursive(cam);
		for (auto&it : modelsUpdate)
			it->updateAsPArent();
		for (auto&it : modelsRendering)
			it->drawOnly(cam);
	}

}